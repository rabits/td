/**
 * @file    CTypeCamera.h
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

#ifndef CTYPECAMERA_H
#define CTYPECAMERA_H

#include "Common.h"

#include <OGRE/Ogre.h>

#include "World/Types/CType.h"
#include "Nerv/CControlled.h"

/** @brief Camera realization for the object
 *
 */
class CTypeCamera
    : public CType
    , public virtual CControlled
{
public:
    /** @brief Default constructor
     *
     */
    CTypeCamera();

    /** @brief Info about type
     *
     */
    void info() const;


    /** @brief Enumerator values for different styles of camera movement
     */
    enum CameraStyle
    {
        CS_FREELOOK, ///< Free fly (without connection point)
        CS_ORBIT,    ///< 3rd person view
        CS_EYES      ///< 1st person view
    };

    /** @brief Swaps the camera on our camera man for another camera.
     *
     * @param cam
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
     * @param target
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
     * @param yaw
     * @param pitch
     * @param dist
     * @return void
     *
     */
    void yawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);


    /** @brief Sets the camera's top speed. Only applies for free-look style
     *
     * @param top_speed
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
     * @param style
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


    /** @brief Update state
     *
     * @param time_since_last_frame
     * @return void
     *
     */
    void update(const Ogre::Real time_since_last_frame);

protected:
    CTypeParameter<uint>        m_Range;
    CTypeParameter<std::string> m_Type;
    CTypeParameter<std::string> m_Person;
    CTypeParameter<uint>        m_LookRangeX;
    CTypeParameter<uint>        m_LookRangeY;

    Ogre::Camera*               m_pCamera;  ///< Controlled camera object

    CameraStyle                 m_Style;
    Ogre::SceneNode*            m_pTarget;

    bool                        m_orbiting;
    bool                        m_zooming;
    Ogre::Real                  m_TopSpeed;
    Ogre::Vector3               m_Velocity;

    Ogre::Vector3               m_ActMove;          ///< Action move
    bool                        m_ActLookUpDown;    ///< Action look up-down
    bool                        m_ActLookLeftRight; ///< Action look left-right
    float                       m_ValLookUpDown;    ///< Value look up-down
    float                       m_ValLookLeftRight; ///< Value look left-right
    bool                        m_ActSpeedUp;       ///< Action speed up

private:
    /** @brief Adding actions
     *
     * @see CControlled::registerActions()
     */
    void registerActions();

    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     *
     */
    CTypeCamera(const CTypeCamera& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CTypeCamera& operator=(const CTypeCamera& obj);
};

#endif // CTYPECAMERA_H
