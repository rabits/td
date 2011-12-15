/**
 * @file    CGame.cpp
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Game master object
 */

#include "CGame.h"
#include "Nerv/CSensor.h"

#include <algorithm>

CGame::CGame()
   : CData("Game")
   , CControlled("Game")
   , m_pSceneMgr()
   , m_pCamera()
   , m_oCurrentWorld()
   , m_pInputHandler()
   , m_pWindow()
   , m_pRoot()
   , m_pLogManager()
   , m_pTimer(new Ogre::Timer())
   , m_NextFrameTime(0)
   , m_Worlds()
   , m_pMainUser()
   , m_Users()
   , m_oCurrentUser()
   , m_ShutDown(false)
{
    m_pTimer->reset();
}

CGame* CGame::s_pInstance = NULL;
fs::path* CGame::s_pPrefix = NULL;

CGame::~CGame()
{
    for( m_oCurrentWorld=m_Worlds.begin() ; m_oCurrentWorld < m_Worlds.end(); m_oCurrentWorld++ )
        delete (*m_oCurrentWorld);
    for( m_oCurrentUser = m_Users.begin() ; m_oCurrentUser < m_Users.end(); m_oCurrentUser++ )
        delete (*m_oCurrentUser);

    delete m_pTimer;

    // Remove debug drawer
    delete DebugDrawer::getSingletonPtr();

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(m_pWindow, this);

    // m_pInputManager will be destructed by windowClosed
    windowClosed(m_pWindow);

    delete m_pRoot;
    delete m_pLogManager;

    delete s_pPrefix;
}

const char* CGame::getPrefix()
{
    if( s_pPrefix == NULL )
    {
        // Set prefix path
        s_pPrefix = new fs::path(Common::getPrefixPath());
        log_info("Prefix path: %s", s_pPrefix->c_str());
    }

    return s_pPrefix->c_str();
}

bool CGame::initialise()
{
    log_info("Start initialisation");

    // Loading environment
    loadEnv();

    // Loading global config
    fs::path config_path(CGame::getPrefix());
    config_path /= CONFIG_PATH_GLOBAL_CONFIG;
    config_path /= "config.xml";
    loadData(config_path.c_str());

    // Loading user config
    config_path = fs::path(env("HOME"));
    config_path /= path("user_data");
    if( config_path == fs::path(env("HOME")) )
    {
        log_warn("User data relative directory not setted in config - using default \"$HOME/.config/td\"");
        m_data.child("path").child("user_data").child(pugi::node_pcdata).set_value(".config/td");
        config_path /= ".config/td";
    }
    if( ! fs::is_directory(config_path) )
    {
        log_warn("Not found user directory \"%s\", creating new", config_path.c_str());
        if( ! fs::create_directories(config_path) )
            return log_error("Could't create user data directory \"%s\"");
    }
    config_path /= "config.xml";
    if( ! loadData(config_path.c_str()) )
        log_notice("Can't load user configuration \"%s\"", config_path.c_str());

    // Loading gettext messages for default locale on this machine
    config_path = CGame::getPrefix() / fs::path(path("root_data")) / path("locale");
    setLocale(config_path.c_str());

    // Initialise OGRE
    initOgre();

    // Initialise Bullet
    initBullet();

    // Initialise OIS
    initOIS();

    // Initialise Sound
    initSound();

    // Initialise Game
    initGame();

#ifdef CONFIG_DEBUG
    log_info("Init debug drawer");
    new DebugDrawer(m_pSceneMgr, 0.5f);
#endif

    log_info("Complete configuration");

    return true;
}

bool CGame::loadEnv()
{
    log_notice("Loading environment");

    pugi::xml_node data_env = m_data.append_child("env");

    // User name
#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
    data_env.append_child("USER").append_child(pugi::node_pcdata).set_value(std::getenv("USER"));
#else
    data_env.append_child("USER").append_child(pugi::node_pcdata).set_value(std::getenv("USERNAME"));
#endif

    // Home of user
    data_env.append_child("HOME").append_child(pugi::node_pcdata).set_value(std::getenv("HOME"));

    return true;
}

bool CGame::initOgre()
{
    log_notice("Initialising OGRE graphic engine");
    pugi::xml_node ogre_config = m_data.child("config").child("ogre");

    // Creating OGRE log
    fs::path log_path(env("HOME"));
    log_path /= path("user_data");
    log_path /= "ogre.log";
    m_pLogManager = new Ogre::LogManager();
#ifdef CONFIG_DEBUG
    m_pLogManager->createLog(log_path.c_str(), true, true, false);
#else
    m_pLogManager->createLog(log_path.c_str(), true, false, false);
#endif

    // Loading root object
    m_pRoot = new Ogre::Root("", "", "");

    // Get plugin directory
    fs::path ogre_plugins_dir(path("ogre_plugins"));
    if( ogre_plugins_dir.empty() )
    {
        ogre_plugins_dir = CGame::getPrefix();
        ogre_plugins_dir /= CONFIG_PATH_PREFIX_BIN;
        log_warn("Not found ogre_plugins path. Trying binary folder \"%s\" for find OGRE plugins", ogre_plugins_dir.c_str());
    }

    log_info("Loading OGRE configuration");
    pugi::xml_node ogre_engine;
    if( ogre_config.child("video").attribute("choice") )
    {
        log_notice("Choiced engine: %s", ogre_config.child("video").attribute("choice").value());
        ogre_engine = ogre_config.child("video").find_child_by_attribute("name", ogre_config.child("video").attribute("choice").value());
    }

    if( ! ogre_engine )
    {
        log_warn("Not found choiced video engine (\"%s\"). Will be used first in video tree.", ogre_engine.name());
        ogre_engine = ogre_config.child("video").child("engine");
    }

    fs::path ogre_plugin(ogre_plugins_dir);
    if( ogre_engine )
    {
        log_info("Found engine %s (from %d engines)", ogre_engine.attribute("name").value(), ogre_config.child("video").num_children("engine"));
        if( ogre_engine.attribute("plugin") )
        {
            ogre_plugin /= ogre_engine.attribute("plugin").value();
            log_info("Loading engine plugin %s", ogre_plugin.c_str());
            m_pRoot->loadPlugin(ogre_plugin.c_str());
        }
        else
            ogre_engine = pugi::xml_node();
    }

    if( ! ogre_engine )
    {
        log_error("No one video plugins in engine config, using default settings");
        // @todo Create default configuration in xml
        ogre_plugin /= "RenderSystem_GL";
        m_pRoot->loadPlugin(ogre_plugin.c_str());
    }

    log_info("Loading render engine and init video");
    auto render_system = m_pRoot->getAvailableRenderers().begin();
    m_pRoot->setRenderSystem(*render_system);
    m_pRoot->initialise(false);

    Ogre::ConfigOptionMap possible_configs = m_pRoot->getRenderSystem()->getConfigOptions();

#ifdef CONFIG_DEBUG
    log_debug("Available options:");
    for( auto it = possible_configs.begin(); it != possible_configs.end(); it++ )
    {
        auto itp = it->second.possibleValues.begin();
        std::string possible_values("'" + *itp++ + "'");
        for( ; itp != it->second.possibleValues.end(); itp++ )
            possible_values += ",'" + *itp + "'";
        log_debug("\t\"%s\" (default: %s) in (%s)", it->first.c_str(), it->second.currentValue.c_str(), possible_values.c_str());
    }
#endif

    log_info("Configuring video output");
    Ogre::NameValuePairList miscParams;

    // Hard default params
    bool fullscreen = false;
    uint x = 800, y = 600;

    // Standart parameters:
    fullscreen = (std::strcmp(ogre_engine.child("display").child_value("full_screen"), "Yes") == 0);
    if( ogre_engine.child("display").child("x") && ogre_engine.child("display").child("y") )
    {
        x = Ogre::StringConverter::parseUnsignedInt(ogre_engine.child("display").child_value("x"));
        y = Ogre::StringConverter::parseUnsignedInt(ogre_engine.child("display").child_value("y"));
    }
    log_info("\tDisplay: %sx%s, fullscreen: %s", ogre_engine.child("display").child_value("x")
             , ogre_engine.child("display").child_value("y")
             , ogre_engine.child("display").child_value("full_screen"));

    // Misc parameters
    pugi::xml_node ogre_misc = ogre_engine.child("misc");
    if( ogre_misc )
    {
        for( auto it = possible_configs.begin(); it != possible_configs.end(); it++ )
        {
            if( it->first.compare("Video Mode") || it->first.compare("Full Screen") )
                continue;

            std::string pname(it->first);
            pname.erase(std::remove_if(pname.begin(), pname.end(), static_cast<int(*)(int)>(std::isspace)), pname.end());
            if( ogre_misc.child(pname.c_str()) )
            {
                std::string pvalue(ogre_misc.child_value(pname.c_str()));
                if( std::find(it->second.possibleValues.begin(), it->second.possibleValues.end(), pvalue) != it->second.possibleValues.end() )
                {
                    log_info("\tMisc set \"%s\" = %s", it->first.c_str(), ogre_misc.child_value(pname.c_str()));
                    miscParams[it->first] = pvalue;
                }
                else
                    log_warn("\tMisc bad parameter \"%s\", using default value '%s'", pname.c_str(), it->second.currentValue.c_str());
            }
        }
    }

    // Special parameters:
    //  Fixed borders for window
    miscParams["border"] = "fixed";

    m_pWindow = m_pRoot->createRenderWindow(CONFIG_TD_FULLNAME, x, y, fullscreen, &miscParams);


    log_info("Loading OGRE additional plugins");
    if( ogre_config.child("plugins").child("plugin") )
    {
        pugi::xml_node plugins = ogre_config.child("plugins");
        for( auto plugin = plugins.begin(); plugin != plugins.end(); plugin++ )
        {
            if( plugin->attribute("value") )
            {
                ogre_plugin = ogre_plugins_dir / plugin->attribute("value").value();
                log_info("\tLoading %s \"%s\"", plugin->name(), ogre_plugin.c_str());
                m_pRoot->loadPlugin(ogre_plugin.c_str());
            }
            else
                log_warn("\tFound empty value plugin in config of OGRE plugins");
        }
    }
    else
        log_warn("\tPlugins not found");


    log_info("Preparing resources");
    pugi::xml_node ogre_resources(ogre_config.child("resources"));
    fs::path ogre_resource_location;
    if( ogre_resources )
    {
        for( auto rg = ogre_resources.begin(); rg != ogre_resources.end(); rg++ )
        {
            log_info("\tGroup \"%s\"", rg->name());
            for( auto res = rg->begin(); res != rg->end(); res++ )
            {
                if( res->attribute("value") )
                {
                    fs::path full_user_data = fs::path(env("HOME")) / fs::path(path("user_data")) / fs::path(path("data"));
                    fs::path full_root_data;
                    ogre_resource_location = full_user_data / fs::path(res->attribute("value").value());
                    if( !fs::exists(ogre_resource_location) )
                    {
                        full_root_data = CGame::getPrefix() / fs::path(path("root_data")) / fs::path(path("data"));
                        ogre_resource_location = full_root_data / fs::path(res->attribute("value").value());
                    }

                    if( fs::exists(ogre_resource_location) )
                    {
                        log_info("\t%s, location \"%s\"", res->name(), ogre_resource_location.c_str());
                        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                                    ogre_resource_location.string(), std::string(res->name()), std::string(rg->name()));
                    }
                    else
                        log_error("\tResource path \"%s\" not exists in user_data (\"%s\") and root_data (\"%s\")"
                                  , res->attribute("value").value(), full_user_data.c_str(), full_root_data.c_str());
                }
                else
                    log_warn("\tFound bad resource without value: type \"%s\"", rg->name(), res->name());
            }
        }
    }

    log_info("Loading all prepared resources");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    return true;
}

bool CGame::initBullet()
{
    log_notice("Initialising Bullet physics engine");

    return true;
}

bool CGame::initOIS()
{
    log_notice("Initialising OIS Nerv controlling system");

    // Create Input handler
    size_t windowHnd = 0;
    m_pWindow->getCustomAttribute("WINDOW", &windowHnd);
    m_pInputHandler = new CSensor(windowHnd);

    //Set initial mouse clipping size
    windowResized(m_pWindow);

    return true;
}

bool CGame::initSound()
{
    log_notice("Initialising Sound engine");

    return true;
}

bool CGame::initGame()
{
    log_notice("Initialising Game");

    log_info("Creating root scene");
    m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR);

    log_info("Creating main camera");
    m_pCamera = m_pSceneMgr->createCamera("MainGameCamera");
    m_pCamera->setPosition(Ogre::Vector3(0.0f, 400.0f, -50.0f));
    m_pCamera->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));
    m_pCamera->setNearClipDistance(0.01f);

    // Create light
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    Ogre::Light* l = m_pSceneMgr->createLight("MainLight");
    l->setPosition(200,200,200);

    log_info("Creating viewport");
    // Create one viewport, entire window
    Ogre::Viewport* vp = m_pWindow->addViewport(m_pCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    m_pCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    // Creating simple game info
    createFrameListener();

    // Create worlds
    log_info("Creating worlds");
    m_Worlds.push_back(new CWorld());

    // Registering actions
    registerActions();

    // Create users after all game initialised - used game actions
    m_pMainUser = new CUser();
    m_Users.push_back(m_pMainUser);

    return true;
}

void CGame::start()
{
    log_notice("Starting game");

    // Now time for fixing framerate
    unsigned long now;

    // Main game loop
    while( !m_ShutDown )
    {
        now = m_pTimer->getMicroseconds();

        // Rendering:
        if( m_NextFrameTime <= now )
        {
            // Get messages
            Ogre::WindowEventUtilities::messagePump();

            // Rendering scene
            m_pRoot->renderOneFrame();
            m_NextFrameTime = now + 16666;
            if( !m_pWindow->isActive() && m_pWindow->isVisible() )
                m_pWindow->update();
        }
    }
}

void CGame::getScreenshot()
{
    m_pWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
}

void CGame::createFrameListener()
{
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);

    // Register frame listener
    m_pRoot->addFrameListener(this);
}

bool CGame::frameRenderingQueued(const Ogre::FrameEvent&)
{
    if(m_pWindow->isClosed())
        return false;

    if(m_ShutDown)
        return false;

    //Need to capture/update each device
    m_pInputHandler->capture();

    return true;
}

void CGame::updateWorlds(const Ogre::Real time_since_last_frame)
{
    for( m_oCurrentWorld=m_Worlds.begin() ; m_oCurrentWorld < m_Worlds.end(); m_oCurrentWorld++ )
        (*m_oCurrentWorld)->update(time_since_last_frame);
}

void CGame::updateUsers(const Ogre::Real time_since_last_frame)
{
    for( m_oCurrentUser = m_Users.begin() ; m_oCurrentUser < m_Users.end(); m_oCurrentUser++ )
        (*m_oCurrentUser)->update(time_since_last_frame);
}

bool CGame::frameStarted(const Ogre::FrameEvent& evt)
{
    // Updating worlds
    updateWorlds(evt.timeSinceLastFrame);
    // Updating users
    updateUsers(evt.timeSinceLastFrame);

#ifdef CONFIG_DEBUG
    DebugDrawer::getSingleton().build();
#endif
    return true;
}

bool CGame::frameEnded(const Ogre::FrameEvent&)
{
#ifdef CONFIG_DEBUG
    DebugDrawer::getSingleton().clear();
#endif
    return true;
}

void CGame::windowResized(Ogre::RenderWindow* rw)
{
    uint width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState& ms = m_pInputHandler->getMouse()->getMouseState();
    ms.width = static_cast<int>(width);
    ms.height = static_cast<int>(height);
}

void CGame::windowClosed(Ogre::RenderWindow* rw)
{
    // Only close for window that created OIS
    if( (rw == m_pWindow) && m_pInputHandler != NULL )
        delete m_pInputHandler;
}

void CGame::registerActions()
{
    addAction('e', "Exit");
    addAction('s', "Screen Shot");
    //addAction("Up");
    //addAction("Down");
    //addAction("Left");
    //addAction("Right");
    //addAction("Yes");
    //addAction("No");
}

void CGame::doAction(char act, CSignal& sig)
{
    if( sig.value() == 1.0 )
    {
        switch(act){
        case 'e':
            log_debug("Exit action");
            exit();
            break;
        case 's':
            log_debug("ScreenShot action");
            getScreenshot();
            break;
        }
    }
}

void CGame::setLocale(const char* messages_path, const char* locale)
{
    std::setlocale(LC_ALL, locale);
    std::setlocale(LC_NUMERIC, "C");
    bindtextdomain(CONFIG_TD_NAME, messages_path);
    textdomain(CONFIG_TD_NAME);
}
