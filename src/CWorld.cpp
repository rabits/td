/**
 * @file    CWorld.cpp
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

#include "CWorld.h"
#include "CGame.h"

CWorld::CWorld(const Ogre::Vector3& pos)
    : CObject("World", *this, pos)
    , m_pPhyWorld()
    , m_pGravityField()
    , m_pDbgDraw()
    , m_pBroadphase()
    , m_pCollisionConfig()
    , m_pDispatcher()
    , m_pSolver()
{
    m_pNode = m_pGame->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(m_Position);

    // Bullet initialisation.
    m_pBroadphase = new btAxisSweep3(btVector3(-10000,-10000,-10000), btVector3(10000,10000,10000), 1024);
    m_pCollisionConfig = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfig);
    m_pSolver = new btSequentialImpulseConstraintSolver();

    m_pPhyWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfig);
    m_pPhyWorld->setGravity(btVector3(0,0,0));
    m_pDbgDraw = new BtOgre::DebugDrawer(m_pGame->m_pSceneMgr->getRootSceneNode(), m_pPhyWorld);

    m_pDbgDraw->setDebugMode(true);
    m_pPhyWorld->setDebugDrawer(m_pDbgDraw);

    m_pGravityField = new CGravityField(this, 20.0f);

    // Create scene
    attachChild(new CObjectKernel(*this, 20, Ogre::Vector3(200.0f, 11.0f, 0.0f)));
    attachChild(new CObjectCube(*this, CObjectCube::CCUBE, Ogre::Vector3(0.0f, 0.0f, 0.0f)));
}

void CWorld::init()
{
}

CWorld::~CWorld()
{
    //Free Bullet stuff
    delete m_pGravityField;
    delete m_pDbgDraw;
    delete m_pPhyWorld;
    delete m_pSolver;
    delete m_pDispatcher;
    delete m_pCollisionConfig;
    delete m_pBroadphase;
}

void CWorld::update(const Ogre::FrameEvent& evt)
{
    // Check ForceFields
    m_pGravityField->catchFieldContact();

    //Update Bullet world. Don't forget the debugDrawWorld() part!
    m_pPhyWorld->stepSimulation(evt.timeSinceLastFrame, 10);
    m_pPhyWorld->debugDrawWorld();

    m_pDbgDraw->step();

    // Update childrens
    for( m_itChildrens = m_Childrens.begin() ; m_itChildrens < m_Childrens.end(); m_itChildrens++ )
        (*m_itChildrens)->update(evt);

    // Clear object in gravity fields map
    m_pGravityField->clearObjectsInGravityField();
}

void CWorld::setObjectState(int iState)
{
}
