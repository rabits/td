/**
 * @file    CObjectKernel.cpp
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


#include "CObjectKernel.h"
#include "CGame.h"

CObjectKernel::CObjectKernel(CWorld& pWorld, const btScalar mass, const Ogre::Vector3& pos)
    : CObject("Kernel", pWorld, pos, mass)
    , CControlled("Kernel")
    , m_ActForward(false)
    , m_ActBackward(false)
    , m_ActLeft(false)
    , m_ActRight(false)
    , m_ActJump(false)
    , m_ForceForward(0.0)
    , m_ForceBackward(0.0)
    , m_ForceLeft(0.0)
    , m_ForceRight(0.0)
    , m_ForceJump(0.0)
    , m_ForceMax(4.5)
    , m_ForceValue(0.0)
{
    registerActions();
}

void CObjectKernel::init()
{
    if( m_pParent != NULL )
    {
        //Create Ogre stuff.
        m_pEntity = m_pGame->m_pSceneMgr->createEntity("objectkernel.mesh");
        m_pNode = m_pParent->node()->createChildSceneNode(m_Position);
        m_pNode->attachObject(m_pEntity);

        //Create shape.
        BtOgre::StaticMeshToShapeConverter converter(m_pEntity);
        m_pShape = converter.createSphere();

        //Calculate inertia.
        btVector3 inertia;
        m_pShape->calculateLocalInertia(m_Mass, inertia);

        //Create BtOgre MotionState (connects Ogre and Bullet).
        m_pState = new BtOgre::RigidBodyState(m_pNode);

        //Create the Body.
        m_pBody = new btRigidBody(m_Mass, m_pState, m_pShape, inertia);
        m_pWorld->m_pPhyWorld->addRigidBody(m_pBody, CObject::DYNAMIC_OBJECT, CObject::DYNAMIC_OBJECT | CObject::STATIC_OBJECT);
        m_pBody->setFriction(6.0f);

        m_pWorld->m_pGravityField->zeroObjectGravity(m_pBody->getBroadphaseProxy()->getUid(), new btVector3(0.0f,0.0f,0.0f));
    }
}

CObjectKernel::~CObjectKernel()
{
}

void CObjectKernel::update(const Ogre::FrameEvent& evt)
{
    // Update gravity
    m_pBody->setGravity(m_pWorld->m_pGravityField->getObjectGravity(m_pBody->getBroadphaseProxy()->getUid()));

    // Draw gravity line
    m_pWorld->m_pDbgDraw->drawLine(m_pBody->getCenterOfMassPosition(), m_pBody->getCenterOfMassPosition() + m_pBody->getGravity()*2, btVector3());

    if( m_ActForward || m_ActBackward || m_ActLeft || m_ActRight || m_ActJump )
    {
        m_pBody->activate();

        // Processing action state
        if( m_ActForward )
            m_ForceForward += 0.1f;
        else
            m_ForceForward -= 0.1f;
        if( m_ActBackward )
            m_ForceBackward += 0.1f;
        else
            m_ForceBackward -= 0.1f;
        if( m_ActLeft )
            m_ForceLeft += 0.1f;
        else
            m_ForceLeft -= 0.1f;
        if( m_ActRight )
            m_ForceRight += 0.1f;
        else
            m_ForceRight -= 0.1f;

        // Validation of action states
        if( m_ForceForward < 0.0f )
            m_ForceForward = 0.0f;
        if( m_ForceBackward < 0.0f )
            m_ForceBackward = 0.0f;
        if( m_ForceLeft < 0.0f )
            m_ForceLeft = 0.0f;
        if( m_ForceRight < 0.0f )
            m_ForceRight = 0.0f;

        if( m_ForceValue > 0.0f )
        {
            if( m_ForceForward > m_ForceMax )
                m_ForceForward = m_ForceMax * m_ForceValue;
            else
                m_ForceForward *= m_ForceValue;
            if( m_ForceBackward > m_ForceMax )
                m_ForceBackward = m_ForceMax * m_ForceValue;
            else
                m_ForceBackward *= m_ForceValue;
            if( m_ForceLeft > m_ForceMax )
                m_ForceLeft = m_ForceMax * m_ForceValue;
            else
                m_ForceLeft *= m_ForceValue;
            if( m_ForceRight > m_ForceMax )
                m_ForceRight = m_ForceMax * m_ForceValue;
            else
                m_ForceRight *= m_ForceValue;
        }
        else
        {
            if( m_ForceForward > m_ForceMax )
                m_ForceForward = m_ForceMax;
            if( m_ForceBackward > m_ForceMax )
                m_ForceBackward = m_ForceMax;
            if( m_ForceLeft > m_ForceMax )
                m_ForceLeft = m_ForceMax;
            if( m_ForceRight > m_ForceMax )
                m_ForceRight = m_ForceMax;
        }

        //log_debug("Cube force: F:%6.3f B:%6.3f L:%6.3f R:%6.3f", m_ForceForward, m_ForceBackward, m_ForceLeft, m_ForceRight);

        // Set rotation
        m_pBody->setAngularVelocity(m_pBody->getAngularVelocity().rotate(m_pBody->getGravity(), 0.1f));
        //m_pBody->setAngularVelocity(btVector3(m_pBody->getAngularVelocity().x(),
        //                                      m_ForceBackward - m_ForceForward,
        //                                      m_ForceLeft - m_ForceRight));
    }

    ODD.drawCuboid(m_pEntity->getBoundingBox().getAllCorners(), Ogre::ColourValue::Red, true);
}

void CObjectKernel::setObjectState(int iState)
{
}

void CObjectKernel::registerActions()
{
    addAction('f', "Move Forward");
    addAction('b', "Move Backward");
    addAction('l', "Move Left");
    addAction('r', "Move Right");
    addAction('j', "Jump");
}

void CObjectKernel::doAction(char act, CSignal& sig)
{
    m_ForceValue = sig.value();

    switch(act){
    case 'f':
        log_debug("Move forward action");
        m_ActForward = (sig.value() > 0) ? true : false;
        break;
    case 'b':
        log_debug("Move backward action");
        m_ActBackward = (sig.value() > 0) ? true : false;
        break;
    case 'l':
        log_debug("Move left action");
        m_ActLeft = (sig.value() > 0) ? true : false;
        break;
    case 'r':
        log_debug("Move right action");
        m_ActRight = (sig.value() > 0) ? true : false;
        break;
    case 'j':
        log_debug("Jump action");
        m_ActJump = (sig.value() > 0) ? true : false;
        break;
    }
}
