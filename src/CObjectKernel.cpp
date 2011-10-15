/** // COBJECTKERNEL_CPP
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
 */

#include "CObjectKernel.h"
#include "CGame.h"

CObjectKernel::CObjectKernel()
{
}

CObjectKernel::CObjectKernel(CGame &pGame, CObjectWorld & pWorld, const btScalar mass, const Ogre::Vector3 &pos)
{
    if( &pGame != NULL )
        setGame(&pGame);
    if( &pWorld != NULL )
        setWorld(&pWorld);

    m_position = pos;
    m_mass = mass;
}

void CObjectKernel::init()
{
    if( m_pParent != NULL )
    {
        //Create Ogre stuff.
        m_pEntity = m_pGame->m_pSceneMgr->createEntity("objectkernel.mesh");
        m_pNode = m_pParent->m_pNode->createChildSceneNode(m_position);
        m_pNode->attachObject(m_pEntity);

        //Create shape.
        BtOgre::StaticMeshToShapeConverter converter(m_pEntity);
        m_pShape = converter.createSphere();

        //Calculate inertia.
        btVector3 inertia;
        m_pShape->calculateLocalInertia(m_mass, inertia);

        //Create BtOgre MotionState (connects Ogre and Bullet).
        m_pState = new BtOgre::RigidBodyState(m_pNode);

        //Create the Body.
        m_pBody = new btRigidBody(m_mass, m_pState, m_pShape, inertia);
        m_pWorld->phyWorld->addRigidBody(m_pBody, CObject::DYNAMIC_OBJECT, CObject::DYNAMIC_OBJECT | CObject::STATIC_OBJECT);
        m_pBody->setFriction(6.0f);

        m_pWorld->m_pGravityField->zeroObjectGravity(m_pBody->getBroadphaseProxy()->getUid(), new btVector3(0.0f,0.0f,0.0f));
    }
}

CObjectKernel::~CObjectKernel()
{
}

void CObjectKernel::update()
{
    // Update gravity
    m_pBody->setGravity(m_pWorld->m_pGravityField->getObjectGravity(m_pBody->getBroadphaseProxy()->getUid()));

    // Draw gravity line
    m_pWorld->dbgdraw->drawLine(m_pBody->getCenterOfMassPosition(), m_pBody->getCenterOfMassPosition() + m_pBody->getGravity()*4, btVector3());

    // Set rotation
    m_pBody->setAngularVelocity(btVector3(-2.5f,m_pBody->getAngularVelocity().y(),m_pBody->getAngularVelocity().z()));
}

void CObjectKernel::setObjectState(int iState)
{
}

