/**
 * @file    CSensor.cpp
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

#include "Nerv/CSensor.h"

#include <string>

CSensor::CSensor(size_t windowHnd)
    : m_pInputManager()
    , m_pMouse()
    , m_pKeyboard()
    , m_pJoyStick()
    , m_JoysticsNum()
    , m_pGame(CGame::getInstance())
    , m_subscribedUsers()
    , m_CleanMouse(-1)
    , m_LastMouseX(0)
    , m_LastMouseY(0)
    , m_LastMouseZ(0)
    , m_LastPovX(0)
    , m_LastPovY(0)
    , m_JoyStickZero(2048)
{
    m_DeviceType[0] = "Unknown";
    m_DeviceType[1] = "Keyboard";
    m_DeviceType[2] = "Mouse";
    m_DeviceType[3] = "JoyStick";
    m_DeviceType[4] = "Tablet";
    m_DeviceType[5] = "Other";

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

    OIS::DeviceList dlist = m_pInputManager->listFreeDevices();
    for( auto i = dlist.begin(); i != dlist.end(); ++i )
        log_info("\tFound device: %s,\tVendor: %s", m_DeviceType[i->first].c_str(), i->second.c_str());

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject( OIS::OISKeyboard, true ));
    m_pKeyboard->setEventCallback(this);
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputManager->createInputObject( OIS::OISMouse, true ));
    m_pMouse->setEventCallback(this);

    try
    {
        m_JoysticsNum = std::min(m_pInputManager->getNumberOfDevices(OIS::OISJoyStick), CONFIG_JOYSTICK_MAX_NUMBER);
        if( m_JoysticsNum > 0 )
        {
            log_info("\tInit %d joysticks:", m_JoysticsNum);
            for( int i = 0; i < m_JoysticsNum; i++ )
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
                // @todo: Use feedback code
#endif //CONFIG_JOYSTICK_USE_FORCEFEEDBACK
            }
        }
    }
    catch(OIS::Exception& ex)
    {
        log_warn("Exception raised on joystick creation: %s", ex.eText);
    }

    // Preparing subscribers map
    std::map<uint, CUser*> nullmap;
    m_subscribedUsers[OIS::OISKeyboard] = nullmap;
    m_subscribedUsers[OIS::OISMouse] = nullmap;
    m_subscribedUsers[OIS::OISJoyStick] = nullmap;
}

CSensor::~CSensor()
{
    log_debug("Destroying Nerv sensor");

    m_pInputManager->destroyInputObject(m_pMouse);
    m_pInputManager->destroyInputObject(m_pKeyboard);
    if( m_JoysticsNum > 0 )
    {
        for( int i = 0; i < m_JoysticsNum; i++ )
            m_pInputManager->destroyInputObject(m_pJoyStick[i]);
    }

    for( auto it = m_subscribedUsers.begin(); it != m_subscribedUsers.end(); it++ )
    {
        for( auto itu = it->second.begin(); itu != it->second.end(); itu++ )
            log_debug("Found subscribe id#%d", itu->first);
    }

    OIS::InputManager::destroyInputSystem(m_pInputManager);

    log_debug("Complete destroying Nerv sensor");
}

void CSensor::capture()
{
    // Capture keyboard
    m_pKeyboard->capture();

    // Clean move and capture of mouse
    if( m_CleanMouse >= 0 )
    {
        m_CleanMouse = m_CleanMouse - 1;
        if( m_CleanMouse == -1 )
        {
            log_debug("Cleaning mouse move %d", m_CleanMouse);
            OIS::MouseState mouse_state = m_pMouse->getMouseState();
            mouse_state.clear();
            OIS::MouseEvent mouse_event(m_pMouse, mouse_state);
            mouseMoved(mouse_event);
        }
    }
    m_pMouse->capture();

    // Capture joystics
    if( m_JoysticsNum > 0 )
    {
        for( int i = 0; i < m_JoysticsNum; i++ )
            m_pJoyStick[i]->capture();
    }
}

OIS::Mouse* CSensor::getMouse()
{
    return m_pMouse;
}

OIS::Keyboard* CSensor::getKeyboard()
{
    return m_pKeyboard;
}

OIS::JoyStick* CSensor::getJoyStick(int joyId)
{
    if( joyId >= 0 && joyId < m_JoysticsNum )
        return m_pJoyStick[joyId];
    else
        log_warn("Joystick with id#%d not present", joyId);

    return NULL;
}

bool CSensor::keyPressed( const OIS::KeyEvent& arg )
{
    // Converting keyboard keypress to nerv Signal
    CSignal sig(genId(OIS::OISKeyboard, 0, arg.key), 1.0);

    SigUser::iterator user = m_subscribedUsers[OIS::OISKeyboard].find(sig.id());
    if( user != m_subscribedUsers[OIS::OISKeyboard].end() )
        user->second->nervSignal(sig);

    // @todo Move control of game to CGame object Actions
    if( arg.key == OIS::KC_T )
    {
        // Cycle filter rendering mode
        Ogre::FilterOptions ft = Ogre::MaterialManager::getSingleton().getDefaultTextureFiltering(Ogre::FT_MIN);
        Ogre::TextureFilterOptions tfo;
        uint aniso;

        switch( ft )
        {
            case Ogre::FO_POINT:
                tfo = Ogre::TFO_TRILINEAR;
                aniso = 1;
                break;
            case Ogre::FO_LINEAR:
                tfo = Ogre::TFO_ANISOTROPIC;
                aniso = 8;
                break;
            default:
                tfo = Ogre::TFO_NONE;
                aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
    }
    else if( arg.key == OIS::KC_R )
    {
        // Cycle polygon rendering mode
        Ogre::PolygonMode pm;

        switch( m_pGame->m_pCamera->getPolygonMode() )
        {
            case Ogre::PM_SOLID:
                pm = Ogre::PM_WIREFRAME;
                break;
            case Ogre::PM_WIREFRAME:
                pm = Ogre::PM_POINTS;
                break;
            default:
                pm = Ogre::PM_SOLID;
        }

        m_pGame->m_pCamera->setPolygonMode(pm);
    }
    else if( arg.key == OIS::KC_F5 )
    {
        // Refresh all textures
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if( arg.key == OIS::KC_Q )
    {
        m_pGame->exit();
    }

    return true;
}

bool CSensor::keyReleased( const OIS::KeyEvent& arg )
{
    // Converting keyboard keyrelease to nerv Signal
    CSignal sig(genId(OIS::OISKeyboard, 0, arg.key), 0.0);

    SigUser::iterator user = m_subscribedUsers[OIS::OISKeyboard].find(sig.id());
    if( user != m_subscribedUsers[OIS::OISKeyboard].end() )
        user->second->nervSignal(sig);

    return true;
}

bool CSensor::mouseMoved( const OIS::MouseEvent& arg )
{
    // @todo Fix left-right and up-down
    if( arg.state.X.rel || arg.state.Y.rel || arg.state.Z.rel )
        m_CleanMouse = 2;

    CSignal sig;

    // Converting mouse move into nerv Signal
    if( arg.state.X.rel != m_LastMouseX )
    {
        if( arg.state.X.rel < 0 )
            sig = CSignal(genId(OIS::OISMouse, 0, SENS_LEFT), static_cast<float>(-arg.state.X.rel), 0.05f);
        else if( arg.state.X.rel > 0 )
            sig = CSignal(genId(OIS::OISMouse, 0, SENS_RIGHT), static_cast<float>(arg.state.X.rel), 0.05f);
        else
            sig = CSignal(genId(OIS::OISMouse, 0, (m_LastMouseX < 0) ? SENS_LEFT : SENS_RIGHT),
                          static_cast<float>(arg.state.X.rel), 0.05f);

        SigUser::iterator user = m_subscribedUsers[OIS::OISMouse].find(sig.id());
        if( user != m_subscribedUsers[OIS::OISMouse].end() )
            user->second->nervSignal(sig);

        m_LastMouseX = arg.state.X.rel;
    }

    if( arg.state.Y.rel != m_LastMouseY )
    {
        if( arg.state.Y.rel < 0 )
            sig = CSignal(genId(OIS::OISMouse, 0, SENS_UP), static_cast<float>(-arg.state.Y.rel), 0.05f);
        else if( arg.state.Y.rel > 0 )
            sig = CSignal(genId(OIS::OISMouse, 0, SENS_DOWN), static_cast<float>(arg.state.Y.rel), 0.05f);
        else
            sig = CSignal(genId(OIS::OISMouse, 0, (m_LastMouseY < 0) ? SENS_UP : SENS_DOWN),
                          static_cast<float>(arg.state.Z.rel), 0.05f);

        SigUser::iterator user = m_subscribedUsers[OIS::OISMouse].find(sig.id());
        if( user != m_subscribedUsers[OIS::OISMouse].end() )
            user->second->nervSignal(sig);

        m_LastMouseY = arg.state.Y.rel;
    }

    if( arg.state.Z.rel != m_LastMouseZ )
    {
        if( arg.state.Z.rel < 0 )
            sig = CSignal(genId(OIS::OISMouse, 0, SENS_IMMERSION), static_cast<float>(-arg.state.Z.rel), 0.004166f);
        else if( arg.state.Z.rel > 0 )
            sig = CSignal(genId(OIS::OISMouse, 0, SENS_EMERSION), static_cast<float>(arg.state.Z.rel), 0.004166f);
        else
            sig = CSignal(genId(OIS::OISMouse, 0, (m_LastMouseZ < 0) ? SENS_IMMERSION : SENS_EMERSION),
                          static_cast<float>(arg.state.Z.rel), 0.05f);

        SigUser::iterator user = m_subscribedUsers[OIS::OISMouse].find(sig.id());
        if( user != m_subscribedUsers[OIS::OISMouse].end() )
            user->second->nervSignal(sig);

        m_LastMouseZ = arg.state.Z.rel;
    }

    return true;
}

bool CSensor::mousePressed( const OIS::MouseEvent&, OIS::MouseButtonID button )
{
    // Converting mouse button press to nerv Signal
    CSignal sig(genId(OIS::OISMouse, 1, button), 1.0);

    SigUser::iterator user = m_subscribedUsers[OIS::OISMouse].find(sig.id());
    if( user != m_subscribedUsers[OIS::OISMouse].end() )
        user->second->nervSignal(sig);

    return true;
}

bool CSensor::mouseReleased( const OIS::MouseEvent&, OIS::MouseButtonID button )
{
    // Converting mouse button release to nerv Signal
    CSignal sig(genId(OIS::OISMouse, 1, button), 0.0);

    SigUser::iterator user = m_subscribedUsers[OIS::OISMouse].find(sig.id());
    if( user != m_subscribedUsers[OIS::OISMouse].end() )
        user->second->nervSignal(sig);

    return true;
}

bool CSensor::povMoved( const OIS::JoyStickEvent& arg, int pov )
{
    // @todo Realize many joysticks
    CSignal sig;

    // X axis
    if( arg.state.mPOV[pov].direction & OIS::Pov::West )
    {
        sig = CSignal(genId(OIS::OISJoyStick, 0, SENS_LEFT), 1.0);
        m_LastPovX = 1;
    }
    else if( arg.state.mPOV[pov].direction & OIS::Pov::East )
    {
        sig = CSignal(genId(OIS::OISJoyStick, 0, SENS_RIGHT), 1.0);
        m_LastPovX = 2;
    }
    else if( m_LastPovX != 0 )
    {
        sig = CSignal(genId(OIS::OISJoyStick, 0, m_LastPovX), 0.0);
        m_LastPovX = 0;
    }

    if( sig.id() != 0 )
    {
        SigUser::iterator user = m_subscribedUsers[OIS::OISJoyStick].find(sig.id());
        if( user != m_subscribedUsers[OIS::OISJoyStick].end() )
            user->second->nervSignal(sig);
    }

    // Y axis
    if( arg.state.mPOV[pov].direction & OIS::Pov::North )
    {
        sig = CSignal(genId(OIS::OISJoyStick, 0, SENS_UP), 1.0);
        m_LastPovY = 3;
    }
    else if( arg.state.mPOV[pov].direction & OIS::Pov::South )
    {
        sig = CSignal(genId(OIS::OISJoyStick, 0, SENS_DOWN), 1.0);
        m_LastPovY = 4;
    }
    else if( m_LastPovY != 0 )
    {
        sig = CSignal(genId(OIS::OISJoyStick, 0, m_LastPovY), 0.0);
        m_LastPovY = 0;
    }

    if( sig.id() != 0 )
    {
        SigUser::iterator user = m_subscribedUsers[OIS::OISJoyStick].find(sig.id());
        if( user != m_subscribedUsers[OIS::OISJoyStick].end() )
            user->second->nervSignal(sig);
    }

    return true;
}

bool CSensor::buttonPressed( const OIS::JoyStickEvent&, int button )
{
    // @todo Realize many joysticks
    // Converting joystick button press to nerv Signal
    CSignal sig(genId(OIS::OISJoyStick, 1, button), 1.0);

    SigUser::iterator user = m_subscribedUsers[OIS::OISJoyStick].find(sig.id());
    if( user != m_subscribedUsers[OIS::OISJoyStick].end() )
        user->second->nervSignal(sig);

    return true;
}

bool CSensor::buttonReleased( const OIS::JoyStickEvent& arg, int button )
{
    // @todo Realize many joysticks
    log_debug("Joystick \"%s\" button #%d released", arg.device->vendor().c_str(), button);

    // Converting joystick button release to nerv Signal
    CSignal sig(genId(OIS::OISJoyStick, 1, button), 0.0);

    SigUser::iterator user = m_subscribedUsers[OIS::OISJoyStick].find(sig.id());
    if( user != m_subscribedUsers[OIS::OISJoyStick].end() )
        user->second->nervSignal(sig);

    return true;
}

bool CSensor::axisMoved( const OIS::JoyStickEvent& arg, int axis )
{
    // @todo Realize many joysticks
    int value = arg.state.mAxes[static_cast<uint>(axis)].abs;

    // Separate axis by sign and change value if needed
    //  Axis 1 need to inverse direction (and id)
    int direct = axis * 2;
    int opp_direct = axis % 2;

    log_debug("Value %d pos %d", value, direct);

    if( opp_direct )
    {
        // Inverse Up-Down axis
        if( value > 0 )
        {
            opp_direct = direct;
            direct++;
        }
        else
        {
            value = (-value) - 1;
            opp_direct = direct + 1;
        }
    }
    else
    {
        // Left-Right axis
        if( value > 0 )
        {
            opp_direct = direct;
            direct++;
        }
        else
        {
            value = (-value) - 1;
            opp_direct = direct - 1;
        }
    }

    if( direct > 15 )
        return log_warn("Stick %d not supported now - maximum is 4 sticks per one Joystick", direct/4);

    CSignal sig;
    if( (std::abs(value) > m_JoyStickZero) )
    {
        m_ChangedAxis[direct] = true;
        sig = CSignal(genId(OIS::OISJoyStick, 2, direct), static_cast<float>(value) / 32767.0f);
    }
    else
    {
        if( m_ChangedAxis[direct] == true )
        {
            m_ChangedAxis[direct] = false;
            sig = CSignal(genId(OIS::OISJoyStick, 2, direct), 0.0);
        }
    }

    if( sig.id() != 0 )
    {
        SigUser::iterator user = m_subscribedUsers[OIS::OISJoyStick].find(sig.id());
        if( user != m_subscribedUsers[OIS::OISJoyStick].end() )
            user->second->nervSignal(sig);
    }

    // Nulling opposite direction
    if( m_ChangedAxis[opp_direct] == true )
    {
        log_debug("Nulling %d", opp_direct);
        m_ChangedAxis[opp_direct] = false;
        sig = CSignal(genId(OIS::OISJoyStick, 2, opp_direct), 0.0);

        SigUser::iterator user = m_subscribedUsers[OIS::OISJoyStick].find(sig.id());
        if( user != m_subscribedUsers[OIS::OISJoyStick].end() )
            user->second->nervSignal(sig);
    }

    return true;
}

bool CSensor::addSubscribe(uint id, CUser* pUser)
{
    uint device = id / 10000u;

    log_debug("Subscribing user \"%s\" to %s signal id#%u", pUser->name().c_str(), m_DeviceType[device].c_str(), id);

    m_subscribedUsers[static_cast<OIS::Type>(device)][id] = pUser;

    return true;
}

bool CSensor::delSubscribe(uint id)
{
    // @todo add recognize type of device by id
    OIS::Type device = OIS::OISKeyboard;
    m_subscribedUsers[device].erase(id);

    return true;
}
