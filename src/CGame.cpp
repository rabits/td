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
    if( m_pTrayMgr )
        delete m_pTrayMgr;
    if( m_pTimer )
        delete m_pTimer;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(m_pWindow, this);
    windowClosed(m_pWindow);

    delete m_pRoot;

    for( o_currentWorld=m_vWorlds.begin() ; o_currentWorld < m_vWorlds.end(); o_currentWorld++ )
        delete (*o_currentWorld);
    for( o_currentUser = m_vUsers.begin() ; o_currentUser < m_vUsers.end(); o_currentUser++ )
        delete (*o_currentUser);
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
    //loadConfig(m_pPrefix);

    //m_pRoot = new Ogre::Root(CONFIG_PATH_CONFIG "plugins.cfg", CONFIG_PATH_CONFIG "main.cfg", CONFIG_PATH_CONFIG "logfile.log");
    m_pRoot = new Ogre::Root();

    //-------- configure ---------------
    log_info("Creating main configuration");
    if( !(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog()) )
        return false;

    m_pRoot->initialise(false);

    Ogre::ConfigOptionMap options = m_pRoot->getRenderSystem()->getConfigOptions();
    Ogre::ConfigOptionMap::iterator opt;
    Ogre::ConfigOptionMap::iterator end = options.end();
    Ogre::NameValuePairList miscParams;

    bool fullscreen = false;
    uint w = 800, h = 600;

    if((opt = options.find("Full Screen")) != end)
        fullscreen = (opt->second.currentValue == "Yes");
    if((opt = options.find("Display Frequency")) != end)
        miscParams["displayFrequency"] = opt->second.currentValue;
    if((opt = options.find("Video Mode")) != end)
    {
        Ogre::String val = opt->second.currentValue;
        Ogre::String::size_type pos = val.find('x');

        if (pos != Ogre::String::npos)
        {
            w = Ogre::StringConverter::parseUnsignedInt(val.substr(0, pos));
            h = Ogre::StringConverter::parseUnsignedInt(val.substr(pos + 1));
        }
    }
    if((opt = options.find("FSAA")) != end)
        miscParams["FSAA"] = opt->second.currentValue;
    if((opt = options.find("VSync")) != end)
        miscParams["vsync"] = opt->second.currentValue;
    if((opt = options.find("sRGB Gamma Conversion")) != end)
        miscParams["gamma"] = opt->second.currentValue;
    miscParams["border"] = "fixed";

    m_pWindow = m_pRoot->createRenderWindow(CONFIG_TD_FULLNAME, w, h, fullscreen, &miscParams);

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

    createFrameListener();

    // Create worlds
    log_info("Creating worlds");
    m_vWorlds.push_back(new CObjectWorld(*this));

    log_info("Complete configuration");

    return true;
}

bool CGame::loadEnv()
{
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
        return log_error("Ferifying of data failed");

    // Starting merge
    pugi::xml_node new_child = new_data.child(CONFIG_TD_NAME).child(m_dataName);
    mergeData(new_child);

#ifdef CONFIG_DEBUG
    log_debug("Data before merge:");
    m_dataBefore.save(std::cout, "  ");
    log_debug("Data After merge:");
    m_dataRoot.save(std::cout, "  ");
#endif

    /*fs::path path_root_share(CONFIG_PATH_ROOT_SHARE);
    data_path.append_child("path_root_share").append_child(pugi::node_pcdata).set_value(path_root_share.c_str());

    if( ! fs::is_directory(path_home_share) )
    {
        log_info("Creating user home config directory");
        boost::filesystem3::create_directories(path_home_share);

        // Creating default configuration of game
        loadConfig();
        pugi::xml_node data_config = m_data.append_child("config");
        saveData(std::cout);
    }
    else
    {

    }*/

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
