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

#include <OGRE/Ogre.h>
#include "Nerv/CControlled.h"

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
    CEye(Ogre::Camera* camera);

    /** @brief Destructor
     */
    virtual ~CEye();

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
    inline void camera(Ogre::Camera* cam) { m_pCamera = cam; }

    /** @brief Get connected camera object
     *
     * @return Ogre::Camera*
     *
     */
    inline Ogre::Camera* camera() { return m_pCamera; }


    /** @brief Sets the target we will revolve around. Only applies for orbit style
     *
     * @param target Ogre::SceneNode*
     * @return void
     *
     */
    void target(Ogre::SceneNode* target);

    /** @brief Get target of camera
     *
     * @return Ogre::SceneNode*
     *
     */
    inline Ogre::SceneNode* target() { return m_pTarget; }

    /** @brief Sets the spatial offset from the target. Only applies for orbit style
     *
     * @param yaw Ogre::Radian
     * @param pitch Ogre::Radian
     * @param dist Ogre::Real
     * @return void
     *
     */
    void yawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);


    /** @brief Sets the camera's top speed. Only applies for free-look style
     *
     * @param top_speed Ogre::Real
     * @return void
     *
     */
    inline void topSpeed(Ogre::Real top_speed) { m_TopSpeed = top_speed; }

    /** @brief Get top speed
     *
     * @return Ogre::Real
     *
     */
    inline Ogre::Real topSpeed() { return m_TopSpeed; }


    /** @brief Sets the movement style of our camera man
     *
     * @param style CameraStyle
     * @return void
     *
     */
    void style(CameraStyle style);

    /** @brief Get camera controlling style
     *
     * @return CameraStyle
     *
     */
    inline CameraStyle style() { return m_Style; }


    /** @brief Manually stops the camera when in free-look mode
     *
     * @return void
     *
     */
    void stop();

    /** @brief Move eye
     *
     * @param one
     */
    void actionMove(Ogre::Vector3 &one);

    /** @brief Speed boost
     *
     * @param act
     */
    void actionSpeedBoost(float act);

    /** @brief Set look
     *
     * @param rel
     */
    void actionLook(Ogre::Vector3 &rel);


    /** @brief Doing need actions
     *
     * @see CControlled::doAction()
     */
    void doAction(char act, CSignal& sig);

protected:
    /** @brief Adding actions
     *
     * @see CControlled::registerActions()
     */
    void registerActions();

    Ogre::Camera*                m_pCamera; ///< Camera object

    Ogre::Vector3                m_cameraPositionLimits; ///< Camera limits of posioion

    CameraStyle m_Style;
    Ogre::SceneNode* m_pTarget;
    bool m_orbiting;
    bool m_zooming;
    Ogre::Real m_TopSpeed;
    Ogre::Vector3 m_Velocity;

    bool m_ActForward;     ///< Action move forward
    bool m_ActBackward;    ///< Action move backward
    bool m_ActLeft;        ///< Action move left
    bool m_ActRight;       ///< Action move right
    bool m_ActUp;          ///< Action move up
    bool m_ActDown;        ///< Action move down
    bool m_ActSpeedUp;     ///< Action speed up

private:
    /** @brief Fake copy constructor
     *
     * @param obj const CEye&
     *
     * @todo create copy constructor
     *
     */
    CEye(const CEye& obj);
    /** @brief Fake eq operator
     *
     * @param obj const CEye&
     *
     * @toto create eq copy operator
     */
    CEye& operator=(const CEye& obj);
};

#endif // CEYE_H
