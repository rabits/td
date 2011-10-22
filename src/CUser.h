/**
 * @file    CUser.h
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   User
 *
 *
 */


#ifndef CUSER_H
#define CUSER_H

#include "config.h"

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include "CControl.h"
#include "CControlled.h"
#include "CObjectWorld.h"

class CUser
    : public CControlled
{
public:
    CUser(Ogre::Camera *cam);
    ~CUser();

    void setControlledObject(CControlled *obj);
    void setWorld(CObjectWorld *world);
    void update(const Ogre::FrameEvent& evt);

    // enumerator values for different styles of camera movement
    enum CameraStyle
    {
        USER_CS_FREELOOK,
        USER_CS_ORBIT,
        USER_CS_MANUAL
    };
    void setCamera(Ogre::Camera* cam);
    Ogre::Camera* getCamera();
    void setTarget(Ogre::SceneNode* target);
    Ogre::SceneNode* getTarget();
    void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);
    void setTopSpeed(Ogre::Real topSpeed);
    Ogre::Real getTopSpeed();
    void setStyle(CameraStyle style);
    CameraStyle getStyle();
    void manualStop();

    // Keyboard events
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );

    // Mouse events
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Joystick events
    bool povMoved( const OIS::JoyStickEvent &arg, int pov );
    bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
    bool buttonReleased( const OIS::JoyStickEvent &arg, int button );
    bool axisMoved( const OIS::JoyStickEvent &arg, int axis );

    void actionMove(Ogre::Vector3 &one);
    void actionSpeedBoost(float act);
    void actionLook(Ogre::Vector3 &rel);

protected:
    bool useKeyboard();
    bool useMouse();
    bool useJoyStick();

    // Name
    Ogre::String                m_name;
    // Control System
    CControl                    *m_pControl;

    // Controlled object
    CControlled                    *m_pControlledObject;
    // Camera object
    Ogre::Camera                *m_pCamera;

    // Misc params:
    //  Camera limits of posioion
    Ogre::Vector3                m_cameraPositionLimits;

    CameraStyle m_style;
    Ogre::SceneNode* m_pTarget;
    bool m_orbiting;
    bool m_zooming;
    Ogre::Real m_topSpeed;
    Ogre::Vector3 m_velocity;
    bool m_goingForward;
    bool m_goingBack;
    bool m_goingLeft;
    bool m_goingRight;
    bool m_goingUp;
    bool m_goingDown;
    bool m_fastMove;

private:
};

#endif // CUSER_H
