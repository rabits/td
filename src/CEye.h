/**
 * @file    CEye.h
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
#ifndef CEYE_H
#define CEYE_H

#include "CControlled.h"

/** @brief It is invisible camera controller
 *
 * Container for camera for connection to controll set
 */
class CEye
    : public CControlled
{
public:
    /** @brief Constructor
     */
    CEye(Ogre::Camera *camera);

    /** @brief Destructor
     */
    ~CEye();

    /** @brief Update eye state
     *
     * @param evt const Ogre::FrameEvent&
     * @return void
     *
     */
    void update(const Ogre::FrameEvent& evt);

    /** @brief Enumerator values for different styles of camera movement
     */
    enum CameraStyle
    {
        EYE_CS_FREELOOK, ///< Free fly (without connection point)
        EYE_CS_ORBIT,    ///< Orbit around controlled object
        EYE_CS_MANUAL    ///< Self-defined style
    };

    /** @brief Swaps the camera on our camera man for another camera.
     *
     * @param cam Ogre::Camera*
     * @return void
     *
     */
    void          setCamera(Ogre::Camera* cam);

    /** @brief Get connected camera object
     *
     * @return Ogre::Camera*
     *
     */
    Ogre::Camera* getCamera();


    /** @brief Sets the target we will revolve around. Only applies for orbit style
     *
     * @param target Ogre::SceneNode*
     * @return void
     *
     */
    void             setTarget(Ogre::SceneNode* target);

    /** @brief Get target of camera
     *
     * @return Ogre::SceneNode*
     *
     */
    Ogre::SceneNode* getTarget();

    /** @brief Sets the spatial offset from the target. Only applies for orbit style
     *
     * @param yaw Ogre::Radian
     * @param pitch Ogre::Radian
     * @param dist Ogre::Real
     * @return void
     *
     */
    void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);


    /** @brief Sets the camera's top speed. Only applies for free-look style
     *
     * @param topSpeed Ogre::Real
     * @return void
     *
     */
    void       setTopSpeed(Ogre::Real topSpeed);

    /** @brief Get top speed
     *
     * @return Ogre::Real
     *
     */
    Ogre::Real getTopSpeed();


    /** @brief Sets the movement style of our camera man
     *
     * @param style CameraStyle
     * @return void
     *
     */
    void        setStyle(CameraStyle style);

    /** @brief Get camera controlling style
     *
     * @return CameraStyle
     *
     */
    CameraStyle getStyle();


    /** @brief Manually stops the camera when in free-look mode
     *
     * @return void
     *
     */
    void manualStop();

    /* Will be replaced by Nerv controlling system

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
    */

protected:
    Ogre::Camera                *m_pCamera; ///< Camera object

    Ogre::Vector3                m_cameraPositionLimits; ///< Camera limits of posioion

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

#endif // CEYE_H
