/** // COBJECTWORLD_H
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
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
