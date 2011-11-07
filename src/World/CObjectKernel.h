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

#include "CControlled.h"
#include "CObject.h"

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
    , CControlled
{
public:
    /** @brief Simple kernel constructor
     */
    CObjectKernel();

    /** @brief Constructor
     *
     * @param pWorld CWorld&
     * @param mass const btScalar
     * @param pos const Ogre::Vector3& (default Ogre::Vector3(0.0f))
     */
    CObjectKernel(CWorld & pWorld, const btScalar mass, const Ogre::Vector3 &pos = Ogre::Vector3(0.0f));

    /** @brief Destructor
     */
    ~CObjectKernel();


    /** @brief Update object data, it may be animation or just object translation or so on
     *
     * @return void
     *
     */
    void update();

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

private:
};

#endif // COBJECTKERNEL_H_INCLUDED
