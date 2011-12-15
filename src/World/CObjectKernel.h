/**
 * @file    CObjectKernel.h
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Kernel world object
 *
 *
 */

#ifndef COBJECTKERNEL_H_INCLUDED
#define COBJECTKERNEL_H_INCLUDED

#include "Common.h"

#include "World/CObject.h"
#include "Nerv/CControlled.h"
#include "World/Types/CTypeCamera.h"
#include "World/Types/CTypeEnergy.h"

/** @brief Sphere - it is minimal bot and kernel of user
 *
 *  This is one-cube bot-controller (is user). If it destroyed - user
 * been killed and can't control any other objects (if placed in special
 * control cube of controlled object).
 *
 *  Sphere can move by rotating (up, down, left, right) in the direction
 * of user camera.
 */
class CObjectKernel
    : public virtual CControlled
    , public CObject
    , public CTypeCamera
{
public:
    /** @brief Constructor
     *
     * @param pWorld
     * @param mass
     * @param pos (default Ogre::Vector3(0.0f))
     */
    CObjectKernel(CWorld& pWorld, const btScalar mass, const Ogre::Vector3& pos = Ogre::Vector3(0.0f));

    /** @brief Destructor
     */
    ~CObjectKernel();


    /** @brief Update object data, it may be animation or just object translation or so on
     *
     * @param time_since_last_frame
     * @return void
     *
     */
    void update(const Ogre::Real time_since_last_frame);

    /** @brief Initialize object
     *
     * @return void
     *
     */
    void init();


    /** @brief Doing need actions
     *
     * @see CControlled::doAction()
     */
    void doAction(char act, CSignal& sig);

    /** @brief Get current direction
     *
     * @return Ogre::Vector3&
     */
    Ogre::Vector3& front();

    /** @brief Set look direction of object
     *
     * @param direct
     */
    void front(Ogre::Vector3& direct);

private:
    /** @brief Adding actions
     *
     * @see CControlled::registerActions()
     */
    void registerActions();

    Ogre::Vector3 m_Front;        ///< Look vector of object
    btVector3 m_Gravity;          ///< Last gravity vector

    Ogre::Vector3 m_ActMove;      ///< Action move
    Ogre::Vector3 m_Velocity;     ///< Current velocity
    Ogre::Quaternion m_Direction; ///< Current rotation state

    float m_SpeedMin;    ///< Maximum speed
    float m_SpeedMax;    ///< Minimum speed
};

#endif // COBJECTKERNEL_H_INCLUDED
