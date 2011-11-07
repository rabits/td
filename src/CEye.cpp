/**
 * @file    CEye.cpp
 * @date    2011-10-24T23:26:46+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Container for camera
 *
 *
 */
#include "CEye.h"

CEye::CEye(Ogre::Camera* camera = NULL)
    : m_pCamera(camera)
    , m_cameraPositionLimits()
    , m_style(EYE_CS_FREELOOK)
    , m_pTarget(0)
    , m_orbiting(false)
    , m_zooming(false)
    , m_topSpeed(150)
    , m_velocity(Ogre::Vector3::ZERO)
    , m_goingForward(false)
    , m_goingBack(false)
    , m_goingLeft(false)
    , m_goingRight(false)
    , m_goingUp(false)
    , m_goingDown(false)
    , m_fastMove(false)
{
//    if( camera == NULL )
//        newCamera();
}

CEye::~CEye()
{
    //dtor
}

void CEye::update(const Ogre::FrameEvent& evt)
{
    if( m_style == EYE_CS_FREELOOK )
    {
        // build our acceleration vector based on keyboard input composite
        Ogre::Vector3 accel = Ogre::Vector3::ZERO;
        if( m_goingForward ) accel += m_pCamera->getDirection();
        if( m_goingBack ) accel -= m_pCamera->getDirection();
        if( m_goingRight ) accel += m_pCamera->getRight();
        if( m_goingLeft ) accel -= m_pCamera->getRight();
        if( m_goingUp ) accel += m_pCamera->getUp();
        if( m_goingDown ) accel -= m_pCamera->getUp();

        // if accelerating, try to reach top speed in a certain time
        Ogre::Real topSpeed = m_fastMove ? m_topSpeed * 5 : m_topSpeed;
        if( accel.squaredLength() != 0 )
        {
            accel.normalise();
            m_velocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
        } // if not accelerating, try to stop in a certain time
        else m_velocity -= m_velocity * evt.timeSinceLastFrame * 10;

        Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

        // keep camera velocity below top speed and above epsilon
        if( m_velocity.squaredLength() > (topSpeed * topSpeed) )
        {
            m_velocity.normalise();
            m_velocity *= topSpeed;
        }
        else if( m_velocity.squaredLength() < (tooSmall * tooSmall) )
            m_velocity = Ogre::Vector3::ZERO;

        if( m_velocity != Ogre::Vector3::ZERO )
            m_pCamera->move(m_velocity * evt.timeSinceLastFrame);
    }
}

void CEye::setCamera(Ogre::Camera* cam)
{
    m_pCamera = cam;
}

Ogre::Camera* CEye::getCamera()
{
    return m_pCamera;
}

void CEye::setTarget(Ogre::SceneNode* target)
{
    if( target != m_pTarget )
    {
        m_pTarget = target;
        if(target)
        {
            setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
            m_pCamera->setAutoTracking(true, m_pTarget);
        }
        else
        {
            m_pCamera->setAutoTracking(false);
        }

    }
}

Ogre::SceneNode* CEye::getTarget()
{
    return m_pTarget;
}

void CEye::setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
{
    m_pCamera->setPosition(m_pTarget->_getDerivedPosition());
    m_pCamera->setOrientation(m_pTarget->_getDerivedOrientation());
    m_pCamera->yaw(yaw);
    m_pCamera->pitch(-pitch);
    m_pCamera->moveRelative(Ogre::Vector3(0, 0, dist));
}

void CEye::setTopSpeed(Ogre::Real topSpeed)
{
    m_topSpeed = topSpeed;
}

Ogre::Real CEye::getTopSpeed()
{
    return m_topSpeed;
}

void CEye::setStyle(CameraStyle style)
{
    if( m_style != EYE_CS_ORBIT && style == EYE_CS_ORBIT )
    {
        setTarget(m_pTarget ? m_pTarget : m_pCamera->getSceneManager()->getRootSceneNode());
        m_pCamera->setFixedYawAxis(true);
        manualStop();
        setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);

    }
    else if( m_style != EYE_CS_FREELOOK && style == EYE_CS_FREELOOK )
    {
        m_pCamera->setAutoTracking(false);
        m_pCamera->setFixedYawAxis(true);
    }
    else if( m_style != EYE_CS_MANUAL && style == EYE_CS_MANUAL )
    {
        m_pCamera->setAutoTracking(false);
        manualStop();
    }
    m_style = style;

}

CEye::CameraStyle CEye::getStyle()
{
    return m_style;
}

void CEye::manualStop()
{
    if( m_style == EYE_CS_FREELOOK )
    {
        m_goingForward = false;
        m_goingBack = false;
        m_goingLeft = false;
        m_goingRight = false;
        m_goingUp = false;
        m_goingDown = false;
        m_velocity = Ogre::Vector3::ZERO;
    }
}

/* Will be replaced by Nerv control system

bool CUser::keyPressed( const OIS::KeyEvent &arg )
{
    if( useKeyboard() )
    {
        if( m_style == USER_CS_FREELOOK )
        {
            if( arg.key == OIS::KC_W || arg.key == OIS::KC_UP ) m_goingForward = true;
            else if( arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN ) m_goingBack = true;
            else if( arg.key == OIS::KC_A || arg.key == OIS::KC_LEFT ) m_goingLeft = true;
            else if( arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT ) m_goingRight = true;
            else if( arg.key == OIS::KC_PGUP ) m_goingUp = true;
            else if( arg.key == OIS::KC_PGDOWN ) m_goingDown = true;
            else if( arg.key == OIS::KC_LSHIFT ) m_fastMove = true;
        }
    }

    return true;
}

bool CUser::keyReleased( const OIS::KeyEvent &arg )
{
    if( useKeyboard() )
    {
        if( m_style == USER_CS_FREELOOK )
        {
            if( arg.key == OIS::KC_W || arg.key == OIS::KC_UP ) m_goingForward = false;
            else if( arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN ) m_goingBack = false;
            else if( arg.key == OIS::KC_A || arg.key == OIS::KC_LEFT ) m_goingLeft = false;
            else if( arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT ) m_goingRight = false;
            else if( arg.key == OIS::KC_PGUP ) m_goingUp = false;
            else if( arg.key == OIS::KC_PGDOWN ) m_goingDown = false;
            else if( arg.key == OIS::KC_LSHIFT ) m_fastMove = false;
        }
    }

    return true;
}

bool CUser::mouseMoved( const OIS::MouseEvent &arg )
{
    if( useMouse() )
    {
//actionLook()
    }

    return true;
}

bool CUser::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if( useMouse() )
    {
        -----------------------------------------------------------------------------
        | Processes mouse presses. Only applies for orbit style.
        | Left button is for orbiting, and right button is for zooming.
        ------------------------------------------------------------------------
        if( m_style == USER_CS_ORBIT )
        {
            if( id == OIS::MB_Left ) m_orbiting = true;
            else if( id == OIS::MB_Right ) m_zooming = true;
        }
    }

    return true;
}

bool CUser::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if( useMouse() )
    {
        -----------------------------------------------------------------------------
        | Processes mouse releases. Only applies for orbit style.
        | Left button is for orbiting, and right button is for zooming.
        ------------------------------------------------------------------------
        if( m_style == USER_CS_ORBIT )
        {
            if( id == OIS::MB_Left ) m_orbiting = false;
            else if( id == OIS::MB_Right ) m_zooming = false;
        }
    }

    return true;
}

bool CUser::povMoved( const OIS::JoyStickEvent &arg, int pov )
{
    if( useJoyStick() )
    {
    }

    return true;
}

bool CUser::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{
    if( useJoyStick() )
    {
    }

    return true;
}

bool CUser::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
    if( useJoyStick() )
    {
    }

    return true;
}

bool CUser::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{
    if( useJoyStick() )
    {
    }

    return true;
}

// ---------- Actions -----------

void CUser::actionMove(Ogre::Vector3 &one)
{
    if( m_style == USER_CS_FREELOOK )
    {
        m_goingForward = (one.z > 0.0f);
        m_goingBack = (one.z < 0.0f);
        m_goingLeft = (one.x < 0.0f);
        m_goingRight = (one.x > 0.0f);
        m_goingUp = (one.y > 0.0f);
        m_goingDown = (one.y < 0.0f);
    }
}

void CUser::actionSpeedBoost(float act)
{
    if( m_style == USER_CS_FREELOOK )
        m_fastMove = (act > 0.0f);
}

void CUser::actionLook(Ogre::Vector3 &rel)
{
    if( m_style == USER_CS_ORBIT )
    {
        Ogre::Real dist = (m_pCamera->getPosition() - m_pTarget->_getDerivedPosition()).length();

        if( m_orbiting )   // yaw around the target, and pitch locally
        {
            m_pCamera->setPosition(m_pTarget->_getDerivedPosition());

            m_pCamera->yaw(Ogre::Degree(-rel.x * 0.25f));
            m_pCamera->pitch(Ogre::Degree(-rel.y * 0.25f));

            m_pCamera->moveRelative(Ogre::Vector3(0, 0, dist));

            // don't let the camera go over the top or around the bottom of the target
        }
        else if( m_zooming )  // move the camera toward or away from the target
        {
            // the further the camera is, the faster it moves
            m_pCamera->moveRelative(Ogre::Vector3(0, 0, rel.y * 0.004f * dist));
        }
        else if( rel.z != 0 )  // move the camera toward or away from the target
        {
            // the further the camera is, the faster it moves
            m_pCamera->moveRelative(Ogre::Vector3(0, 0, -rel.z * 0.0008f * dist));
        }
    }
    else if( m_style == USER_CS_FREELOOK )
    {
        m_pCamera->yaw(Ogre::Degree(-rel.x * 0.15f));
        m_pCamera->pitch(Ogre::Degree(-rel.y * 0.15f));
    }
}
*/
