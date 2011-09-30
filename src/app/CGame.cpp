/** // CGAME_CPP
 *  ---------------------------
 *   Total Destruction Project
 *   Sergej Parshev       2010
 *  ---------------------------
 */

#include "app/CGame.h"
#include "user/CInputHandler.h"

/**
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

//-------------------------------------------------------------------------------------
CGame::CGame()
   : m_pSceneMgr(0),
   m_pCamera(0),
   m_pWindow(0),
   m_pInputHandler(0),
   m_pTrayMgr(0),
   m_pDetailsPanel(0),
   m_pRoot(0),
   m_pTimer(0),
   m_NextFrameTime(0),
   m_ShutDown(false)
{
}

CGame* CGame::m_pInstance = NULL;

CGame* CGame::getInstance()
{
    if( !m_pInstance )
        m_pInstance = new CGame();
    return m_pInstance;
}

//-------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------
void CGame::updateWorlds(const Ogre::FrameEvent& evt)
{
    for( o_currentWorld=m_vWorlds.begin() ; o_currentWorld < m_vWorlds.end(); o_currentWorld++ )
        (*o_currentWorld)->update(evt);
}

//-------------------------------------------------------------------------------------
void CGame::updateUsers(const Ogre::FrameEvent& evt)
{
    for( o_currentUser = m_vUsers.begin() ; o_currentUser < m_vUsers.end(); o_currentUser++ )
        (*o_currentUser)->update(evt);
}

//-------------------------------------------------------------------------------------
bool CGame::setup()
{
    printf("asdasd\n");
    Ogre::LogManager::getSingletonPtr()->logMessage("Starting configuration");
    printf("asd\n");
    m_pRoot = new Ogre::Root(CONFIG_PATH_CONFIG "plugins.cfg", CONFIG_PATH_CONFIG "main.cfg", CONFIG_PATH_CONFIG "logfile.log");

    //-------- setupResources ----------
    Ogre::LogManager::getSingletonPtr()->logMessage("Setup resources");
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(CONFIG_PATH_CONFIG "resources.cfg");

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

    //-------- configure ---------------
    Ogre::LogManager::getSingletonPtr()->logMessage("Creating main configuration");
    if( !(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog()) )
        return false;
    m_pWindow = m_pRoot->initialise(true, "TD");
    m_pTimer = new Ogre::Timer();
    //-------- !configure! ---------------

    //-------- chooseSceneManager-------
    Ogre::LogManager::getSingletonPtr()->logMessage("Creating root scene");
    m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_GENERIC);
    //-------- !chooseSceneManager!-------

    //-------- createCamera -----------
    Ogre::LogManager::getSingletonPtr()->logMessage("Creating camera");
    // Create the camera
    m_pCamera = m_pSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    m_pCamera->setPosition(Ogre::Vector3(30,60,30));
    // Look back along -Z
    m_pCamera->lookAt(Ogre::Vector3(0,50,0));
    m_pCamera->setNearClipDistance(0.01);

    m_vUsers.push_back(new CUser(m_pCamera));   // create a default camera controller
    //-------- !createCamera! -----------

    //-------- createViewports --------
    Ogre::LogManager::getSingletonPtr()->logMessage("Creating viewport");
    // Create one viewport, entire window
    Ogre::Viewport* vp = m_pWindow->addViewport(m_pCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    m_pCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    //-------- !createViewports! --------

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    //------- loadResources -----------
    Ogre::LogManager::getSingletonPtr()->logMessage("Loading resources");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    //------- !loadResources! -----------

    createFrameListener();

    // Create worlds
    Ogre::LogManager::getSingletonPtr()->logMessage("Creating worlds");
    m_vWorlds.push_back(new CObjectWorld(*this));

    Ogre::LogManager::getSingletonPtr()->logMessage("Complete configuration");
    
    return true;
};

//-------------------------------------------------------------------------------------
void CGame::start()
{
    Ogre::LogManager::getSingletonPtr()->logMessage("Starting game");
    //m_pRoot->startRendering();

    // Prepare to rendering
    unsigned long now;
    m_pTimer->reset();

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

//-------------------------------------------------------------------------------------
void CGame::getScreenshot()
{
    m_pWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
}

//-------------------------------------------------------------------------------------
void CGame::exit()
{
    m_ShutDown = true;
}

//-------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------
bool CGame::frameStarted(const Ogre::FrameEvent &evt)
{
    return true;
}

//Adjust mouse clipping area
void CGame::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = m_pInputHandler->getMouse()->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void CGame::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( (rw == m_pWindow) && m_pInputHandler != NULL )
    {
        delete m_pInputHandler;
        m_pInputHandler = 0;
    }
}
