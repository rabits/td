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

#include "CObject.h"
#include "Nerv/CControlled.h"

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
    : public CObject
    , public CControlled
{
public:
    /** @brief Constructor
     *
     * @param pWorld CWorld&
     * @param mass const btScalar
     * @param pos const Ogre::Vector3& (default Ogre::Vector3(0.0f))
     */
    CObjectKernel(CWorld& pWorld, const btScalar mass, const Ogre::Vector3& pos = Ogre::Vector3(0.0f));

    /** @brief Destructor
     */
    ~CObjectKernel();


    /** @brief Update object data, it may be animation or just object translation or so on
     *
     * @param evt const Ogre::FrameEvent&
     * @return void
     *
     */
    void update(const Ogre::FrameEvent& evt);

    /** @brief Initialize object
     *
     * @return void
     *
     */
    void init();

    /** @brief Setting up object state
     *
     * @param State int
     * @return void
     *
     */
    void setObjectState(int State);


    /** @brief Doing need actions
     *
     * @see CControlled::doAction()
     */
    void doAction(char act, CSignal& sig);

private:
    /** @brief Adding actions
     *
     * @see CControlled::registerActions()
     */
    void registerActions();

    bool m_ActForward;     ///< Action move forward
    bool m_ActBackward;    ///< Action move backward
    bool m_ActLeft;        ///< Action move left
    bool m_ActRight;       ///< Action move right
    bool m_ActJump;        ///< Action jump
    float m_ForceForward;  ///< Force move forward
    float m_ForceBackward; ///< Force move backward
    float m_ForceLeft;     ///< Force move left
    float m_ForceRight;    ///< Force move right
    float m_ForceJump;     ///< Force jump

    float m_ForceMax;    ///< Maximum force
    float m_ForceValue;  ///< User's action force value
};

#endif // COBJECTKERNEL_H_INCLUDED
