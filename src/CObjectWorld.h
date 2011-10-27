/**
 * @file    CObjectWorld.h
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   World object
 *
 *
 */

#ifndef COBJECTWORLD_H_INCLUDED
#define COBJECTWORLD_H_INCLUDED

#include "Common.h"

#include "CObject.h"
#include "CGravityField.h"

#include "btogre/BtOgrePG.h"
#include "btogre/BtOgreGP.h"
#include "btogre/BtOgreExtras.h"

/** @brief World object
 */
class CObjectWorld : public CObject
{
public:
    /** @brief Constructor
     */
    CObjectWorld();

    /** @brief Copy constructor
     *
     * @param obj const CObjectWorld&
     *
     */
    CObjectWorld(const CObjectWorld &obj);

    /** @brief Constructor
     *
     * @param pGame CGame&
     * @param pos const Ogre::Vector3& (default Ogre::Vector3(0.0f))
     */
    CObjectWorld(CGame &pGame, const Ogre::Vector3 &pos = Ogre::Vector3(0.0f));

    /** @brief Destructor
     */
    ~CObjectWorld();


    /** @brief Update object data, it may be animation or just object translation or so on
     *
     * @param evt const Ogre::FrameEvent&
     * @return void
     *
     */
    void update(const Ogre::FrameEvent& evt);

    /** @brief Update from CObject
     *
     * @return void
     *
     */
    void update(){}

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

    btDiscreteDynamicsWorld              *m_pPhyWorld;     ///< Physical World
    CGravityField                        *m_pGravityField; ///< World gravity field
    BtOgre::DebugDrawer                  *m_pDbgDraw;      ///< Debug drawer

private:
    btAxisSweep3                         *m_pBroadphase;      ///< Bullet broadphase
    btDefaultCollisionConfiguration      *m_pCollisionConfig; ///< Bullet collision config
    btCollisionDispatcher                *m_pDispatcher;      ///< Bullet dispatcher
    btSequentialImpulseConstraintSolver  *m_pSolver;          ///< Bullet solver
};

#endif // COBJECTWORLD_H_INCLUDED
