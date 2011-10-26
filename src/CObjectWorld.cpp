/**
 * @file    CObjectWorld.cpp
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

#include "CObjectWorld.h"
#include "CGame.h"

CObjectWorld::CObjectWorld(CGame &pGame, const Ogre::Vector3 &pos)
    : m_pPhyWorld(NULL)
    , m_pGravityField(NULL)
    , m_pDbgDraw(NULL)
    , m_pBroadphase(NULL)
    , m_pCollisionConfig(NULL)
    , m_pDispatcher(NULL)
    , m_pSolver(NULL)
{
    if( &pGame != NULL )
    {
        setGame(&pGame);
        m_position = pos;

        m_pNode = m_pGame->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(m_position);

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
    delete m_pSolver;
    delete m_pDispatcher;
    delete m_pCollisionConfig;
    delete m_pBroadphase;
}

void CObjectWorld::update(const Ogre::FrameEvent& evt)
{
    // Check ForceFields
    m_pGravityField->catchFieldContact();

    //Update Bullet world. Don't forget the debugDrawWorld() part!
    m_pPhyWorld->stepSimulation(evt.timeSinceLastFrame, 10);
    m_pPhyWorld->debugDrawWorld();

    m_pDbgDraw->step();

    // Update childrens
    for( m_itChildrenList = m_ChildrenList.begin() ; m_itChildrenList < m_ChildrenList.end(); m_itChildrenList++ )
        (*m_itChildrenList)->update();

    // Clear object in gravity fields map
    m_pGravityField->clearObjectsInGravityField();
}

void CObjectWorld::setObjectState(int iState)
{
}
