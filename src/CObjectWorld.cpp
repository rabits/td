/** // COBJECTWORLD_CPP
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
 */

#include "CObjectWorld.h"
#include "CGame.h"

CObjectWorld::CObjectWorld()
{
}

CObjectWorld::CObjectWorld(CGame &pGame, const Ogre::Vector3 &pos)
{
    if( &pGame != NULL )
    {
        setGame(&pGame);
        m_position = pos;

        m_pNode = m_pGame->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(m_position);

        // Bullet initialisation.
        mBroadphase = new btAxisSweep3(btVector3(-10000,-10000,-10000), btVector3(10000,10000,10000), 1024);
        mCollisionConfig = new btDefaultCollisionConfiguration();
        mDispatcher = new btCollisionDispatcher(mCollisionConfig);
        mSolver = new btSequentialImpulseConstraintSolver();

        phyWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfig);
        phyWorld->setGravity(btVector3(0,0,0));
        dbgdraw = new BtOgre::DebugDrawer(m_pGame->m_pSceneMgr->getRootSceneNode(), phyWorld);

        dbgdraw->setDebugMode(true);
        phyWorld->setDebugDrawer(dbgdraw);

        m_pGravityField= new CGravityField(this, 20.0f);

        // Create scene
        attachChild(new CObjectKernel(*m_pGame, *this, 20, Ogre::Vector3(5.0f, 11.0f, 0.0f)));
        attachChild(new CObjectCube(*m_pGame, *this, CObjectCube::CUBE, Ogre::Vector3(0.0f, 0.0f, 0.0f)));
    }
}

void CObjectWorld::init()
{
}

CObjectWorld::~CObjectWorld()
{
    //Free Bullet stuff.
    delete mSolver;
    delete mDispatcher;
    delete mCollisionConfig;
    delete mBroadphase;
}

void CObjectWorld::update(const Ogre::FrameEvent& evt)
{
    // Check ForceFields
    m_pGravityField->catchFieldContact();

    //Update Bullet world. Don't forget the debugDrawWorld() part!
    phyWorld->stepSimulation(evt.timeSinceLastFrame, 10);
    phyWorld->debugDrawWorld();

    dbgdraw->step();

    // Update childrens
    for( m_itChildrenList = m_ChildrenList.begin() ; m_itChildrenList < m_ChildrenList.end(); m_itChildrenList++ )
        (*m_itChildrenList)->update();

    // Clear object in gravity fields map
    m_pGravityField->clearObjectInGravityField();
}

void CObjectWorld::setObjectState(int iState)
{
}
