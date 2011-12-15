/**
 * @file    CTypeCamera.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type camera
 *
 *
 */

#include "World/Types/CTypeCamera.h"

CTypeCamera::CTypeCamera()
    : CControlled("Camera")
    , CType("Camera", "Displays world around")
    , m_Range("Range", "Maximum view distance", 1, 65535)
    , m_Type("Type", "Display data format")
    , m_Person("Person", "View person")
    , m_LookRangeX("Look Range X", "Maximum horizontal free in degrees", 0, 360)
    , m_LookRangeY("Look Range Y", "Maximum vertical free in degrees", 0, 180)
    , m_pCamera(NULL)
    , m_Style(CS_FREELOOK)
    , m_pTarget(NULL)
    , m_orbiting(true)
    , m_zooming(false)
    , m_TopSpeed(150)
    , m_Velocity(Ogre::Vector3::ZERO)
    , m_ActMove(Ogre::Vector3::ZERO)
    , m_ActLookUpDown(false)
    , m_ActLookLeftRight(false)
    , m_ValLookUpDown(0.0f)
    , m_ValLookLeftRight(0.0f)
    , m_ActSpeedUp(false)
{
    m_Type.addAvailable("Simple", "Simplificity view");
    m_Type.addAvailable("Normal", "Normal real view");
    m_Type.addAvailable("Night", "Night vision");
    m_Type.addAvailable("InfraRed", "View in IR diapasone - displays temperature of objects");
    m_Type.addAvailable("UltraViolet", "View in UV diapasone - unvisible markers and other");

    m_Person.addAvailable("Third", "1rd view from eyes");
    m_Person.addAvailable("First", "3rd view from back");

    registerActions();
}

void CTypeCamera::info() const
{
    log_debug("Camera: %s", m_Type.info().c_str());
}

void CTypeCamera::update(const Ogre::Real time_since_last_frame)
{
    if( m_Style == CS_FREELOOK )
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
            m_Velocity += accel * topSpeed * time_since_last_frame * 10;
        }
        else
            m_Velocity -= m_Velocity * time_since_last_frame * 10;

        Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

        if( m_Velocity.squaredLength() > (topSpeed * topSpeed) )
        {
            m_Velocity.normalise();
            m_Velocity *= topSpeed;
        }
        else if( m_Velocity.squaredLength() < (tooSmall * tooSmall) )
            m_Velocity = Ogre::Vector3::ZERO;

        if( m_Velocity != Ogre::Vector3::ZERO )
            m_pCamera->move(m_Velocity * time_since_last_frame);
    }
    else if( m_Style == CS_ORBIT )
    {
        //m_pCamera->setPosition(m_pTarget->getPosition() + 20);
    }

    if( m_ActLookUpDown || m_ActLookLeftRight )
    {
        Ogre::Vector3 look_direction(m_ValLookLeftRight, m_ValLookUpDown, 0.0f);
        actionLook(look_direction);
    }
}

void CTypeCamera::target(Ogre::SceneNode* target)
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
            m_pCamera->setAutoTracking(false);
    }
}

void CTypeCamera::yawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
{
    m_pCamera->setPosition(m_pTarget->_getDerivedPosition());
    m_pCamera->setOrientation(m_pTarget->_getDerivedOrientation());
    m_pCamera->yaw(yaw);
    m_pCamera->pitch(-pitch);
    m_pCamera->moveRelative(Ogre::Vector3(0, 0, dist));
}

void CTypeCamera::style(CameraStyle style)
{
    if( m_Style != CS_ORBIT && style == CS_ORBIT )
    {
        target(m_pTarget ? m_pTarget : m_pCamera->getSceneManager()->getRootSceneNode());
        m_pCamera->setFixedYawAxis(true);
        stop();
        yawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);

    }
    else if( m_Style != CS_FREELOOK && style == CS_FREELOOK )
    {
        m_pCamera->setAutoTracking(false);
        m_pCamera->setFixedYawAxis(true);
    }
    else if( m_Style != CS_EYES && style == CS_EYES )
    {
        m_pCamera->setAutoTracking(false);
        stop();
    }
    m_Style = style;
}

void CTypeCamera::stop()
{
    if( m_Style == CS_FREELOOK )
    {
        m_ActMove = Ogre::Vector3::ZERO;
        m_Velocity = Ogre::Vector3::ZERO;
    }
}

void CTypeCamera::registerActions()
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

void CTypeCamera::doAction(char act, CSignal& sig)
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

void CTypeCamera::actionMove(Ogre::Vector3& one)
{
    if( m_Style == CS_FREELOOK )
        m_ActMove = one;
}

void CTypeCamera::actionSpeedBoost(float act)
{
    if( m_Style == CS_FREELOOK )
        m_ActSpeedUp = (act > 0.0f);
}

void CTypeCamera::actionLook(Ogre::Vector3& rel)
{
    if( m_Style == CS_ORBIT )
    {
        Ogre::Real dist = (m_pCamera->getPosition() - m_pTarget->_getDerivedPosition()).length();

        if( m_orbiting )   // yaw around the target, and pitch locally
        {
            m_pCamera->setPosition(m_pTarget->_getDerivedPosition());

            m_pCamera->yaw(Ogre::Degree(-rel.x * 4.0f));
            m_pCamera->pitch(Ogre::Degree(-rel.y * 4.0f));

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
    else if( m_Style == CS_FREELOOK )
    {
        m_pCamera->yaw(Ogre::Degree(-rel.x * 4.0f));
        m_pCamera->pitch(Ogre::Degree(-rel.y * 4.0f));
    }
}
