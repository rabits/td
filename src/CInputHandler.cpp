/**
 * @file    CInputHandler.cpp
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Input handler
 *
 *
 */

#include "CInputHandler.h"

#include <string>

CInputHandler::CInputHandler(size_t windowHnd)
    : m_pInputManager()
    , m_pMouse()
    , m_pKeyboard()
    , m_pJoyStick()
    , m_joysticsNum()
    , m_pGame(CGame::getInstance())
{
    OIS::ParamList pl;
    std::ostringstream windowHndStr;
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    // Create non-exclusive input
/*#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
#else
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
#endif*/

    m_pInputManager = OIS::InputManager::createInputSystem(pl);

    std::string deviceType[6] = {"OIS Unknown", "OIS Keyboard", "OIS Mouse", "OIS JoyStick", "OIS Tablet", "OIS Other"};
    OIS::DeviceList dlist = m_pInputManager->listFreeDevices();
    for( OIS::DeviceList::iterator i = dlist.begin(); i != dlist.end(); ++i )
        log_info("\tFound device: %s,\tVendor: %s", deviceType[i->first].c_str(), i->second.c_str());

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject( OIS::OISKeyboard, true ));
    m_pKeyboard->setEventCallback(this);
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputManager->createInputObject( OIS::OISMouse, true ));
    m_pMouse->setEventCallback(this);

    try
    {
        m_joysticsNum = std::min(m_pInputManager->getNumberOfDevices(OIS::OISJoyStick), CONFIG_JOYSTICK_MAX_NUMBER);
        if( m_joysticsNum > 0 )
        {
            log_info("\tInit %d joysticks:", m_joysticsNum);
            for( int i = 0; i < m_joysticsNum; i++ )
            {
                m_pJoyStick[i] = static_cast<OIS::JoyStick*>(m_pInputManager->createInputObject( OIS::OISJoyStick, true ));
                m_pJoyStick[i]->setEventCallback(this);
                log_info("\t\tCreating Joystick #%d", i);
                log_info("\t\t\tPOV/HATs: %d", m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_POV));
                log_info("\t\t\tButtons:  %d", m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Button));
                log_info("\t\t\tAxes:     %d", m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Axis));
                log_info("\t\t\tSliders (Not Used): %d", m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Slider));
                log_info("\t\t\tVector3 (Not Used): %d", m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Vector3));

#ifdef CONFIG_JOYSTICK_USE_FORCEFEEDBACK
                // TODO: Use feedback code
#endif //CONFIG_JOYSTICK_USE_FORCEFEEDBACK
            }
        }
    }
    catch(OIS::Exception &ex)
    {
        log_warn("Exception raised on joystick creation: %s", ex.eText);
    }

    // Preparing subscribers map
    std::map<int, CUser*> nullmap;
    m_subscribedUsers[OIS::OISKeyboard] = nullmap;
    m_subscribedUsers[OIS::OISMouse] = nullmap;
    m_subscribedUsers[OIS::OISJoyStick] = nullmap;
}

CInputHandler::~CInputHandler()
{
    m_pInputManager->destroyInputObject(m_pMouse);
    m_pInputManager->destroyInputObject(m_pKeyboard);
    if( m_joysticsNum > 0 )
    {
        for( int i = 0; i < m_joysticsNum; i++ )
            m_pInputManager->destroyInputObject(m_pJoyStick[i]);
    }

    for( std::map<OIS::Type, std::map<int, CUser*> >::iterator it = m_subscribedUsers.begin(); it != m_subscribedUsers.end(); it++ )
    {
        for( std::map<int, CUser*>::iterator itu = it->second.begin(); itu != it->second.end(); itu++ )
            log_notice("Found subscribe id#%d", itu->first);
    }

    OIS::InputManager::destroyInputSystem(m_pInputManager);
}

void CInputHandler::capture()
{
    m_pKeyboard->capture();
    m_pMouse->capture();
    if( m_joysticsNum > 0 )
    {
        for( int i = 0; i < m_joysticsNum; i++ )
            m_pJoyStick[i]->capture();
    }
}

OIS::Mouse* CInputHandler::getMouse()
{
    return m_pMouse;
}

OIS::Keyboard* CInputHandler::getKeyboard()
{
    return m_pKeyboard;
}

OIS::JoyStick* CInputHandler::getJoyStick(int joyId)
{
    if( joyId >= 0 && joyId < m_joysticsNum )
        return m_pJoyStick[joyId];
    else
        log_warn("Joystick with id#%d not present", joyId);

    return NULL;            // WARNING! May return NULL if joystick not present!
}

bool CInputHandler::keyPressed( const OIS::KeyEvent &arg )
{
    log_debug("Key pressed: %s (%d)", (static_cast<OIS::Keyboard*>(const_cast<OIS::Object*>(arg.device)))->getAsString(arg.key).c_str(), arg.key);

    // Converting keyboard keypress to InputEvent
    int id = OIS::OISKeyboard * 10000 + arg.key;
    CInputEvent event(id, 1.0);

//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->keyPressed(arg);

    // @todo Move control of game to CGame object Actions

    if( m_pGame->m_pTrayMgr->isDialogVisible() ) return true;   // don't process any more keys if dialog is up

    if( arg.key == OIS::KC_F )   // toggle visibility of advanced frame stats
    {
        m_pGame->m_pTrayMgr->toggleAdvancedFrameStats();
    }
    else if( arg.key == OIS::KC_G )   // toggle visibility of even rarer debugging details
    {
        if( m_pGame->m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE )
        {
            m_pGame->m_pTrayMgr->moveWidgetToTray(m_pGame->m_pDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            m_pGame->m_pDetailsPanel->show();
        }
        else
        {
            m_pGame->m_pTrayMgr->removeWidgetFromTray(m_pGame->m_pDetailsPanel);
            m_pGame->m_pDetailsPanel->hide();
        }
    }
    else if( arg.key == OIS::KC_T )   // cycle polygon rendering mode
    {
        std::string newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch( m_pGame->m_pDetailsPanel->getParamValue(9).asUTF8()[0] )
        {
            case 'B':
                newVal = "Trilinear";
                tfo = Ogre::TFO_TRILINEAR;
                aniso = 1;
                break;
            case 'T':
                newVal = "Anisotropic";
                tfo = Ogre::TFO_ANISOTROPIC;
                aniso = 8;
                break;
            case 'A':
                newVal = "None";
                tfo = Ogre::TFO_NONE;
                aniso = 1;
                break;
            default:
                newVal = "Bilinear";
                tfo = Ogre::TFO_BILINEAR;
                aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        m_pGame->m_pDetailsPanel->setParamValue(9, newVal);
    }
    else if( arg.key == OIS::KC_R )   // cycle polygon rendering mode
    {
        std::string newVal;
        Ogre::PolygonMode pm;

        switch( m_pGame->m_pCamera->getPolygonMode() )
        {
            case Ogre::PM_SOLID:
                newVal = "Wireframe";
                pm = Ogre::PM_WIREFRAME;
                break;
            case Ogre::PM_WIREFRAME:
                newVal = "Points";
                pm = Ogre::PM_POINTS;
                break;
            default:
                newVal = "Solid";
                pm = Ogre::PM_SOLID;
        }

        m_pGame->m_pCamera->setPolygonMode(pm);
        m_pGame->m_pDetailsPanel->setParamValue(10, newVal);
    }
    else if( arg.key == OIS::KC_F5 )   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if( arg.key == OIS::KC_SYSRQ )   // take a screenshot
    {
        m_pGame->getScreenshot();
    }
    else if( arg.key == OIS::KC_ESCAPE )
    {
        m_pGame->exit();
    }

    return true;
}

bool CInputHandler::keyReleased( const OIS::KeyEvent &arg )
{
//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->keyReleased(arg);

    return true;
}

bool CInputHandler::mouseMoved( const OIS::MouseEvent &arg )
{
    log_debug("MouseMoved: Abs(%d,%d,%d) Rel(%d,%d,%d)", arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs
              , arg.state.X.rel, arg.state.Y.rel, arg.state.Z.rel);

//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->mouseMoved(arg);

    return true;
}

bool CInputHandler::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    log_debug("Mouse pressed #%d", id);

//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->mousePressed(arg, id);

    return true;
}

bool CInputHandler::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->mouseReleased(arg, id);

    return true;
}

bool CInputHandler::povMoved( const OIS::JoyStickEvent &arg, int pov )
{
    // Log
    std::string out = "";
    if( arg.state.mPOV[pov].direction & OIS::Pov::North ) //Going up
        out += "North";
    else if( arg.state.mPOV[pov].direction & OIS::Pov::South ) //Going down
        out += "South";
    if( arg.state.mPOV[pov].direction & OIS::Pov::East ) //Going right
        out += "East";
    else if( arg.state.mPOV[pov].direction & OIS::Pov::West ) //Going left
        out += "West";
    if( arg.state.mPOV[pov].direction == OIS::Pov::Centered ) //stopped/centered out
        out += "Centered";
    log_debug("Joystick \"%s\" POV #%d moved: %s", arg.device->vendor().c_str(), pov, out.c_str());

//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->povMoved(arg, pov);

    return true;
}

bool CInputHandler::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{
    log_debug("Joystick \"%s\" button #%d pressed", arg.device->vendor().c_str(), button);

//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->buttonPressed(arg, button);

    return true;
}

bool CInputHandler::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->buttonReleased(arg, button);

    return true;
}

bool CInputHandler::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{
    log_debug("Joystick \"%s\" Axis #%d moved, Value: %d", arg.device->vendor().c_str(), axis, arg.state.mAxes[axis].abs);

//    for( m_pGame->m_oCurrentUser = m_pGame->m_Users.begin() ; m_pGame->m_oCurrentUser < m_pGame->m_Users.end(); m_pGame->m_oCurrentUser++ )
//        (*m_pGame->m_oCurrentUser)->axisMoved(arg, axis);

    return true;
}

bool CInputHandler::addSubscribe(OIS::Type device, int id, CUser* pUser)
{
    m_subscribedUsers[device][id] = pUser;

    return true;
}

bool CInputHandler::delSubscribe(OIS::Type device, int id)
{
    m_subscribedUsers[device].erase(id);
}
