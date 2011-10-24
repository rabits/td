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
    : m_pInputManager(0)
    , m_pMouse(0)
    , m_pKeyboard(0)
    , m_pJoyStick({0,0,0,0})
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

    m_pGame = CGame::getInstance();

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

    Ogre::String deviceType[6] = {"OIS Unknown", "OIS Keyboard", "OIS Mouse", "OIS JoyStick", "OIS Tablet", "OIS Other"};
    OIS::DeviceList dlist = m_pInputManager->listFreeDevices();
    for( OIS::DeviceList::iterator i = dlist.begin(); i != dlist.end(); ++i )
        Ogre::LogManager::getSingletonPtr()->logMessage("\tFound device: "+deviceType[i->first]+"\tVendor: "+i->second);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject( OIS::OISKeyboard, true ));
    m_pKeyboard->setEventCallback(this);
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputManager->createInputObject( OIS::OISMouse, true ));
    m_pMouse->setEventCallback(this);

    try
    {
        m_joysticsNum = std::min(m_pInputManager->getNumberOfDevices(OIS::OISJoyStick), CONFIG_JOYSTICK_MAX_NUMBER);
        if( m_joysticsNum > 0 )
        {
            Ogre::LogManager::getSingletonPtr()->logMessage("\tInit joysticks ("+Ogre::StringConverter::toString(m_joysticsNum)+"):");
            for( int i = 0; i < m_joysticsNum; i++ )
            {
                m_pJoyStick[i] = static_cast<OIS::JoyStick*>(m_pInputManager->createInputObject( OIS::OISJoyStick, true ));
                m_pJoyStick[i]->setEventCallback(this);
                Ogre::LogManager::getSingletonPtr()->logMessage("\tCreating Joystick #"+Ogre::StringConverter::toString(i));
                Ogre::LogManager::getSingletonPtr()->logMessage("\t\tPOV/HATs: "+Ogre::StringConverter::toString(m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_POV)));
                Ogre::LogManager::getSingletonPtr()->logMessage("\t\tButtons: "+Ogre::StringConverter::toString(m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Button)));
                Ogre::LogManager::getSingletonPtr()->logMessage("\t\tAxes: "+Ogre::StringConverter::toString(m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Axis)));
                Ogre::LogManager::getSingletonPtr()->logMessage("\t\tSliders (Not Used): "+Ogre::StringConverter::toString(m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Slider)));
                Ogre::LogManager::getSingletonPtr()->logMessage("\t\tVector3 (Not Used): "+Ogre::StringConverter::toString(m_pJoyStick[i]->getNumberOfComponents(OIS::OIS_Vector3)));

#ifdef CONFIG_JOYSTICK_USE_FORCEFEEDBACK
                // TODO: Use feedback code
#endif //CONFIG_JOYSTICK_USE_FORCEFEEDBACK
            }
        }
    }
    catch(OIS::Exception &ex)
    {
        Ogre::LogManager::getSingletonPtr()->logMessage("\nException raised on joystick creation: "+Ogre::StringConverter::toString(ex.eText));
    }

    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS Complete ***");
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

    OIS::InputManager::destroyInputSystem(m_pInputManager);
    m_pInputManager = 0;
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
        Ogre::LogManager::getSingletonPtr()->logMessage("Joystick with id#"+Ogre::StringConverter::toString(joyId)+" not preset", Ogre::LML_NORMAL);

    return NULL;            // WARNING! May return NULL if joystick not present!
}

bool CInputHandler::keyPressed( const OIS::KeyEvent &arg )
{
    // Log
    Ogre::LogManager::getSingletonPtr()->logMessage(((OIS::Keyboard*)(arg.device))->getAsString(arg.key));

    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->keyPressed(arg);

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
        Ogre::String newVal;
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
        Ogre::String newVal;
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
    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->keyReleased(arg);

    return true;
}

bool CInputHandler::mouseMoved( const OIS::MouseEvent &arg )
{
    // Log
    Ogre::LogManager::getSingletonPtr()->logMessage("MouseMoved: Abs("+Ogre::StringConverter::toString(arg.state.X.abs)
                                                    +", "+Ogre::StringConverter::toString(arg.state.Y.abs)
                                                    +", "+Ogre::StringConverter::toString(arg.state.Z.abs)
                                                    +") Rel("+Ogre::StringConverter::toString(arg.state.X.rel)
                                                    +", "+Ogre::StringConverter::toString(arg.state.Y.rel)
                                                    +", "+Ogre::StringConverter::toString(arg.state.Z.rel)+")");

    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->mouseMoved(arg);

    return true;
}

bool CInputHandler::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    // Log
    Ogre::LogManager::getSingletonPtr()->logMessage("Mouse button #"+Ogre::StringConverter::toString(id)+" pressed.");

    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->mousePressed(arg, id);

    return true;
}

bool CInputHandler::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->mouseReleased(arg, id);

    return true;
}

bool CInputHandler::povMoved( const OIS::JoyStickEvent &arg, int pov )
{
    // Log
    Ogre::String out = arg.device->vendor()+". POV"+Ogre::StringConverter::toString(pov)+" ";
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
    Ogre::LogManager::getSingletonPtr()->logMessage(out);

    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->povMoved(arg, pov);

    return true;
}

bool CInputHandler::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{
    // Log
    Ogre::LogManager::getSingletonPtr()->logMessage("'"+arg.device->vendor()+"' Button Pressed #"+
                                                    Ogre::StringConverter::toString(button));

    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->buttonPressed(arg, button);

    return true;
}

bool CInputHandler::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->buttonReleased(arg, button);

    return true;
}

bool CInputHandler::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{
    // Log
    Ogre::LogManager::getSingletonPtr()->logMessage("'"+arg.device->vendor()
                                                    +"'. Axis # "+Ogre::StringConverter::toString(axis)
                                                    +" Value: "+Ogre::StringConverter::toString(arg.state.mAxes[axis].abs));

    for( m_pGame->o_currentUser = m_pGame->m_vUsers.begin() ; m_pGame->o_currentUser < m_pGame->m_vUsers.end(); m_pGame->o_currentUser++ )
        (*m_pGame->o_currentUser)->axisMoved(arg, axis);

    return true;
}
