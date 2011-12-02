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

#include "CGame.h"

CEye::CEye(Ogre::Camera* camera)
    : CControlled("Eye")
    , m_pCamera(camera)
    , m_cameraPositionLimits()
    , m_Style(EYE_CS_FREELOOK)
    , m_pTarget()
    , m_orbiting(false)
    , m_zooming(false)
    , m_TopSpeed(150)
    , m_Velocity(Ogre::Vector3::ZERO)
    , m_ActMove(Ogre::Vector3::ZERO)
    , m_ActSpeedUp(false)
    , m_ActLookUpDown(false)
    , m_ActLookLeftRight(false)
    , m_ValLookUpDown(0.0f)
    , m_ValLookLeftRight(0.0f)
{
    registerActions();
}

CEye::~CEye()
{
}

void CEye::update(const Ogre::FrameEvent& evt)
{
    if( m_Style == EYE_CS_FREELOOK )
    {
        Ogre::Vector3 accel = Ogre::Vector3::ZERO;
        if( m_ActMove.z > 0.0f ) accel += m_pCamera->getDirection();
        if( m_ActMove.z < 0.0f ) accel -= m_pCamera->getDirection();
        if( m_ActMove.x > 0.0f ) accel += m_pCamera->getRight();
        if( m_ActMove.x < 0.0f ) accel -= m_pCamera->getRight();
        if( m_ActMove.y > 0.0f ) accel += m_pCamera->getUp();
        if( m_ActMove.y < 0.0f ) accel -= m_pCamera->getUp();

        Ogre::Real topSpeed = m_ActSpeedUp ? m_TopSpeed * 5 : m_TopSpeed;
        if( accel.squaredLength() != 0 )
        {
            accel.normalise();
            m_Velocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
        }
        else
            m_Velocity -= m_Velocity * evt.timeSinceLastFrame * 10;

        Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

        if( m_Velocity.squaredLength() > (topSpeed * topSpeed) )
        {
            m_Velocity.normalise();
            m_Velocity *= topSpeed;
        }
        else if( m_Velocity.squaredLength() < (tooSmall * tooSmall) )
            m_Velocity = Ogre::Vector3::ZERO;

        if( m_Velocity != Ogre::Vector3::ZERO )
            m_pCamera->move(m_Velocity * evt.timeSinceLastFrame);

        if( m_ActLookUpDown || m_ActLookLeftRight )
        {
            Ogre::Vector3 look_direction(m_ValLookLeftRight, m_ValLookUpDown, 0.0f);
            actionLook(look_direction);
        }
    }
}

void CEye::target(Ogre::SceneNode* target)
{
    if( target != m_pTarget )
    {
        m_pTarget = target;
        if(target)
        {
            yawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
            m_pCamera->setAutoTracking(true, m_pTarget);
        }
        else
        {
            m_pCamera->setAutoTracking(false);
        }

    }
}

void CEye::yawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
{
    m_pCamera->setPosition(m_pTarget->_getDerivedPosition());
    m_pCamera->setOrientation(m_pTarget->_getDerivedOrientation());
    m_pCamera->yaw(yaw);
    m_pCamera->pitch(-pitch);
    m_pCamera->moveRelative(Ogre::Vector3(0, 0, dist));
}

void CEye::style(CameraStyle style)
{
    if( m_Style != EYE_CS_ORBIT && style == EYE_CS_ORBIT )
    {
        target(m_pTarget ? m_pTarget : m_pCamera->getSceneManager()->getRootSceneNode());
        m_pCamera->setFixedYawAxis(true);
        stop();
        yawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);

    }
    else if( m_Style != EYE_CS_FREELOOK && style == EYE_CS_FREELOOK )
    {
        m_pCamera->setAutoTracking(false);
        m_pCamera->setFixedYawAxis(true);
    }
    else if( m_Style != EYE_CS_MANUAL && style == EYE_CS_MANUAL )
    {
        m_pCamera->setAutoTracking(false);
        stop();
    }
    m_Style = style;

}

void CEye::stop()
{
    if( m_Style == EYE_CS_FREELOOK )
    {
        m_ActMove = Ogre::Vector3::ZERO;
        m_Velocity = Ogre::Vector3::ZERO;
    }
}

void CEye::registerActions()
{
    addAction('f', "Move Forward");
    addAction('b', "Move Backward");
    addAction('l', "Move Left");
    addAction('r', "Move Right");
    addAction('u', "Move Up");
    addAction('d', "Move Down");
    addAction('s', "Speed Up");
    addAction('U', "Look Up");
    addAction('D', "Look Down");
    addAction('L', "Look Left");
    addAction('R', "Look Right");
}

void CEye::doAction(char act, CSignal& sig)
{
    if( m_Style == EYE_CS_FREELOOK )
    {
        switch(act){
        case 'f':
            m_ActMove.z = sig.value();
            break;
        case 'b':
            m_ActMove.z = -sig.value();
            break;
        case 'l':
            m_ActMove.x = -sig.value();
            break;
        case 'r':
            m_ActMove.x = sig.value();
            break;
        case 'u':
            m_ActMove.y = sig.value();
            break;
        case 'd':
            m_ActMove.y = -sig.value();
            break;
        case 's':
            m_ActSpeedUp = (sig.value() > 0) ? true : false;
            break;
        case 'U':
            m_ActLookUpDown = (sig.value() > 0) ? true : false;
            m_ValLookUpDown = -sig.value();
            break;
        case 'D':
            m_ActLookUpDown = (sig.value() > 0) ? true : false;
            m_ValLookUpDown = sig.value();
            break;
        case 'L':
            m_ActLookLeftRight = (sig.value() > 0) ? true : false;
            m_ValLookLeftRight = -sig.value();
            break;
        case 'R':
            m_ActLookLeftRight = (sig.value() > 0) ? true : false;
            m_ValLookLeftRight = sig.value();
            break;
        }
    }
}

/* Will be replaced by Nerv control system

bool CUser::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if( useMouse() )
    {
        -----------------------------------------------------------------------------
        | Processes mouse presses. Only applies for orbit style.
        | Left button is for orbiting, and right button is for zooming.
        ------------------------------------------------------------------------
        if( m_Style == EYE_CS_ORBIT )
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
        if( m_Style == EYE_CS_ORBIT )
        {
            if( id == OIS::MB_Left ) m_orbiting = false;
            else if( id == OIS::MB_Right ) m_zooming = false;
        }
    }

*/

void CEye::actionMove(Ogre::Vector3& one)
{
    if( m_Style == EYE_CS_FREELOOK )
        m_ActMove = one;
}

void CEye::actionSpeedBoost(float act)
{
    if( m_Style == EYE_CS_FREELOOK )
        m_ActSpeedUp = (act > 0.0f);
}

void CEye::actionLook(Ogre::Vector3& rel)
{
    if( m_Style == EYE_CS_ORBIT )
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
            m_pCamera->moveRelative(Ogre::Vector3(0, 0, rel.y * dist));
        }
        else if( rel.z != 0 )  // move the camera toward or away from the target
        {
            // the further the camera is, the faster it moves
            m_pCamera->moveRelative(Ogre::Vector3(0, 0, -rel.z * dist));
        }
    }
    else if( m_Style == EYE_CS_FREELOOK )
    {
        m_pCamera->yaw(Ogre::Degree(-rel.x * 4));
        m_pCamera->pitch(Ogre::Degree(-rel.y * 4));
    }
}
