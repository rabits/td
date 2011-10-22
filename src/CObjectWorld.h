/**
 * @file    CObjectWorld.h
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   World object
 *
 *
 */


#ifndef COBJECTWORLD_H_INCLUDED
#define COBJECTWORLD_H_INCLUDED

#include "CObject.h"
#include "CGravityField.h"

#include "btogre/BtOgrePG.h"
#include "btogre/BtOgreGP.h"
#include "btogre/BtOgreExtras.h"

#include "config.h"

class CObjectWorld : public CObject
{
public:
    CObjectWorld();
    CObjectWorld(CGame &pGame, const Ogre::Vector3 &pos = Ogre::Vector3(0.0f));
    ~CObjectWorld();

    // Update object data, it may be animation or just object translation or so on.
    void update(const Ogre::FrameEvent& evt);
    // Update from CObject
    void update(){};
    // Initialize object
    void init();
    // Seeting up object state.
    void setObjectState(int State);

    // Physical World
    btDiscreteDynamicsWorld                *phyWorld;
    // Gravity Field Container
    CGravityField                        *m_pGravityField;

    BtOgre::DebugDrawer                    *dbgdraw;

private:
    // Bullet Stuff
    btAxisSweep3                        *mBroadphase;
    btDefaultCollisionConfiguration        *mCollisionConfig;
    btCollisionDispatcher                *mDispatcher;
    btSequentialImpulseConstraintSolver    *mSolver;
};

#endif // COBJECTWORLD_H_INCLUDED
