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
 *
 * Controls:
 *
 * F - toggle visibility of advanced frame stats
 * G - toggle visibility of even rarer debugging details
 * T - cycle polygon rendering mode
 *  B - Trilinear
 *  T - Anisotropic
 *  A - None
 *  Default - Billenear
 * R - cycle polygon rendering mode (Wireframe, Points, Solid)
 * F5 - refresh all textures
 * SysRQ - screenshot
 * Esc - Quit
 */

#include "CGame.h"
#include "CInputHandler.h"

#include <algorithm>

CGame::CGame()
   : CData("Game")
   , m_pSceneMgr(NULL)
   , m_pCamera(NULL)
   , m_pWindow(NULL)
   , m_pInputHandler(NULL)
   , m_pTrayMgr(NULL)
   , m_pDetailsPanel(NULL)
   , m_vUsers()
   , o_currentUser(NULL)
   , o_currentWorld(NULL)
   , m_vWorlds()
   , m_pRoot(NULL)
   , m_pLogManager(NULL)
   , m_pTimer(new Ogre::Timer())
   , m_NextFrameTime(0)
   , m_ShutDown(false)
{
    m_pTimer->reset();
}

CGame* CGame::m_pInstance = NULL;
fs::path* CGame::m_pPrefix = NULL;

CGame::~CGame()
{
    for( o_currentWorld=m_vWorlds.begin() ; o_currentWorld < m_vWorlds.end(); o_currentWorld++ )
        delete (*o_currentWorld);
    for( o_currentUser = m_vUsers.begin() ; o_currentUser < m_vUsers.end(); o_currentUser++ )
        delete (*o_currentUser);

    if( m_pTrayMgr )
        delete m_pTrayMgr;
    if( m_pTimer )
        delete m_pTimer;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(m_pWindow, this);
    windowClosed(m_pWindow);

    if( m_pRoot )
        delete m_pRoot;
    if( m_pLogManager )
        delete m_pLogManager;
}

CGame* CGame::getInstance()
{
    if( m_pInstance == NULL )
        m_pInstance = new CGame();

    return m_pInstance;
}

void CGame::destroyInstance()
{
    if( m_pInstance )
        delete m_pInstance;
};

unsigned int CGame::getTime()
{
    return m_pTimer->getMilliseconds();
}

const char* CGame::getPrefix()
{
    if( m_pPrefix == NULL )
    {
        // Set prefix path
        m_pPrefix = new fs::path(Common::getPrefixPath());
        log_info("Prefix path: %s", m_pPrefix->c_str());
    }

    return m_pPrefix->c_str();
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
    loadConfig(config_path.c_str());

    // Loading user config
    config_path = fs::path(m_data.child("env").child_value("HOME"));
    config_path /= m_data.child("path").child_value("user_data");
    if( config_path == fs::path(m_data.child("env").child_value("HOME")) )
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
    if( ! loadConfig(config_path.c_str()) )
        log_notice("Can't load user configuration \"%s\"", config_path.c_str());

    // Initialise OGRE
    initOgre();

    // Initialise Bullet
    initBullet();

    // Initialise OIS
    initOIS();

    // Initialise Sound
    initSound();

    createFrameListener();

    // Create worlds
    log_info("Creating worlds");
    m_vWorlds.push_back(new CObjectWorld(*this));

    log_info("Complete configuration");

    return true;
}

bool CGame::loadEnv()
{
    log_info("Loading environment");
    pugi::xml_node data_env = m_data.append_child("env");

    // Home of user
    data_env.append_child("HOME").append_child(pugi::node_pcdata).set_value(std::getenv("HOME"));

    return true;
}

bool CGame::loadConfig(const char *configfile)
{
    log_info("Loading game configuration file: \"%s\"", configfile);

    pugi::xml_document new_data;
    pugi::xml_parse_result result = new_data.load_file(configfile, pugi::parse_full);

#ifdef CONFIG_DEBUG
    log_debug("New data for merge:");
    new_data.save(std::cout, "  ");
#endif

    if( !result )
        return log_error("\tLoading failed: %s#%d", result.description(), result.offset);

    // Verify config
    if( ! verifyData(new_data) )
        return log_error("\tFerifying of data failed");

    // Starting merge
    pugi::xml_node new_child = new_data.child(CONFIG_TD_NAME).child(m_dataName);
    mergeData(new_child);

#ifdef CONFIG_DEBUG
    log_debug("Data before merge:");
    m_dataBefore.save(std::cout, "  ");
    log_debug("Data After merge:");
    m_dataRoot.save(std::cout, "  ");
#endif

    log_info("Complete loading game configuration file: \"%s\"", configfile);
    return true;
}

bool CGame::initOgre()
{
    log_info("Initialising OGRE graphic engine");
    pugi::xml_node ogre_config = m_data.child("config").child("ogre");

    // Creating OGRE log
    fs::path log_path(m_data.child("env").child_value("HOME"));
    log_path /= m_data.child("path").child_value("user_data");
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
    fs::path ogre_video_plugin(m_data.child("path").child_value("ogre_plugins"));
    if( ogre_video_plugin.empty() )
    {
        ogre_video_plugin = *m_pPrefix;
        ogre_video_plugin /= CONFIG_PATH_PREFIX_BIN;
        log_warn("Not found ogre_plugins path. Trying binary folder \"%s\" for find OGRE plugins", ogre_video_plugin.c_str());
    }

    log_info("Loading OGRE configuration");
    pugi::xml_node ogre_engine;
    if( ogre_config.child("video").attribute("choice").value() != "" )
    {
        log_notice("Choiced engine: %s", ogre_config.child("video").attribute("choice").value());
        ogre_engine = ogre_config.child("video").find_child_by_attribute("name", ogre_config.child("video").attribute("choice").value());
    }

    if( ! ogre_engine )
    {
        log_warn("Not found choiced video engine (\"%s\"). Will be used first in video tree.", ogre_engine.name());
        ogre_engine = ogre_config.child("video").child("engine");
    }

    if( ogre_engine )
    {
        log_info("Found engine %s (from %d engines)", ogre_engine.attribute("name").value(), ogre_config.child("video").num_children("engine"));
        if( ogre_engine.attribute("plugin").value() == "" )
            ogre_engine = pugi::xml_node();
        else
        {
            ogre_video_plugin /= ogre_engine.attribute("plugin").value();
            log_info("Loading engine plugin %s", ogre_video_plugin.c_str());
            m_pRoot->loadPlugin(ogre_video_plugin.c_str());
        }
    }

    if( ! ogre_engine )
    {
        log_error("No one video plugins in engine config, using default settings");
        // @todo Create default configuration in xml
        ogre_video_plugin /= "RenderSystem_GL";
        m_pRoot->loadPlugin(ogre_video_plugin.c_str());
    }

    log_info("Loading render engine and init video");
    Ogre::RenderSystemList::const_iterator render_system = m_pRoot->getAvailableRenderers().begin();
    m_pRoot->setRenderSystem(*render_system);
    m_pRoot->initialise(false);

    Ogre::ConfigOptionMap possible_configs = m_pRoot->getRenderSystem()->getConfigOptions();

#ifdef CONFIG_DEBUG
    log_debug("Available options:");
    for( Ogre::ConfigOptionMap::const_iterator it = possible_configs.begin(); it != possible_configs.end(); it++ )
    {
        Ogre::StringVector::const_iterator itp = it->second.possibleValues.begin();
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
    fullscreen = (ogre_engine.child("display").child_value("full_screen") == "Yes");
    if( (ogre_engine.child("display").child_value("x") != "") && (ogre_engine.child("display").child_value("y") != "") )
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
        for( Ogre::ConfigOptionMap::const_iterator it = possible_configs.begin(); it != possible_configs.end(); it++ )
        {
            if( (it->first == "Video Mode") || (it->first == "Full Screen") )
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

    log_info("Loading OGRE misk plugins");
    if( m_data.child("config").child("ogre").child("plugins").child("plugin") )
    {
        pugi::xml_node plugins = m_data.child("config").child("ogre").child("plugins").child("plugin");
        for (pugi::xml_node plugin = plugins.child("plugin"); plugin; plugin = plugins.next_sibling("plugin"))
        {
            if( plugin.attribute("value").value() == "" )
                log_warn("Found empty value plugin in config of OGRE plugins");
            else
                m_pRoot->loadPlugin(plugin.attribute("value").value());
        }
    }

    //-------- !configure! ---------------

    //-------- chooseSceneManager-------
    log_info("Creating root scene");
    m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_GENERIC);
    //-------- !chooseSceneManager!-------

    //-------- createCamera -----------
    log_info("Creating camera");
    // Create the camera
    m_pCamera = m_pSceneMgr->createCamera("MainGameCamera");

    // Position it at 500 in Z direction
    m_pCamera->setPosition(Ogre::Vector3(30.0,60.0,30.0));
    // Look back along -Z
    m_pCamera->lookAt(Ogre::Vector3(0.0,50.0,0.0));
    m_pCamera->setNearClipDistance(0.01f);

    //m_vUsers.push_back(new CUser(m_pCamera));   // create a default camera controller
    //-------- !createCamera! -----------

    //-------- createViewports --------
    log_info("Creating viewport");
    // Create one viewport, entire window
    Ogre::Viewport* vp = m_pWindow->addViewport(m_pCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    m_pCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    //-------- !createViewports! --------

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    //-------- setupResources ----------
    log_info("Setup resources");
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            archName, typeName, secName);
        }
    }
    //-------- !setupResources! ----------

    //------- loadResources -----------
    log_info("Loading resources");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    //------- !loadResources! -----------

    return true;
}

bool CGame::initBullet()
{
    log_info("Initialising Bullet physics engine");

    return true;
}

bool CGame::initOIS()
{
    log_info("Initialising OIS Nerv controlling system");

    return true;
}

bool CGame::initSound()
{
    log_info("Initialising Sound engine");

    return true;
}

void CGame::start()
{
    log_notice("Starting game");
    //m_pRoot->startRendering();

    // Prepare to rendering
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

void CGame::exit()
{
    m_ShutDown = true;
}

void CGame::getScreenshot()
{
    m_pWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
}

void CGame::createFrameListener()
{
    size_t windowHnd = 0;
    m_pWindow->getCustomAttribute("WINDOW", &windowHnd);

    // Create Input handler
    m_pInputHandler = new CInputHandler(windowHnd);

    //Set initial mouse clipping size
    windowResized(m_pWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);

    m_pTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", m_pWindow, m_pInputHandler->getMouse(), this);
    m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    m_pTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    m_pDetailsPanel = m_pTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    m_pDetailsPanel->setParamValue(9, "Bilinear");
    m_pDetailsPanel->setParamValue(10, "Solid");
    m_pDetailsPanel->hide();

    m_pRoot->addFrameListener(this);
}

bool CGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
//    kernelObj->update();
    if(m_pWindow->isClosed())
        return false;

    if(m_ShutDown)
        return false;

    //Need to capture/update each device
    m_pInputHandler->capture();

    m_pTrayMgr->frameRenderingQueued(evt);

    // Updating worlds
    updateWorlds(evt);
    // Updating users
    updateUsers(evt);

    if( !m_pTrayMgr->isDialogVisible() )
    {
        if( m_pDetailsPanel->isVisible() )   // if details panel is visible, then update its contents
        {
            m_pDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().x));
            m_pDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().y));
            m_pDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().z));
            m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().w));
            m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().x));
            m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().y));
            m_pDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().z));
        }
    }

    return true;
}

void CGame::updateWorlds(const Ogre::FrameEvent& evt)
{
    for( o_currentWorld=m_vWorlds.begin() ; o_currentWorld < m_vWorlds.end(); o_currentWorld++ )
        (*o_currentWorld)->update(evt);
}

void CGame::updateUsers(const Ogre::FrameEvent& evt)
{
    for( o_currentUser = m_vUsers.begin() ; o_currentUser < m_vUsers.end(); o_currentUser++ )
        (*o_currentUser)->update(evt);
}

bool CGame::frameStarted(const Ogre::FrameEvent &evt)
{
    return true;
}

void CGame::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = m_pInputHandler->getMouse()->getMouseState();
    ms.width = static_cast<int>(width);
    ms.height = static_cast<int>(height);
}

void CGame::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( (rw == m_pWindow) && m_pInputHandler != NULL )
    {
        delete m_pInputHandler;
        m_pInputHandler = 0;
    }
}
