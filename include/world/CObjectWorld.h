/** // COBJECTWORLD_H
 *  ---------------------------
 *   Total Destruction Project
 *   Sergej Parshev       2010
 *  ---------------------------
 */

#ifndef COBJECTWORLD_H_INCLUDED
#define COBJECTWORLD_H_INCLUDED

#include "object/CObject.h"
#include "world/CGravityField.h"

#include "physics/BtOgrePG.h"
#include "physics/BtOgreGP.h"
#include "physics/BtOgreExtras.h"

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
