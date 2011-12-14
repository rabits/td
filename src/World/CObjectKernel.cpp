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

#include "World/CObjectKernel.h"
#include "CGame.h"

CObjectKernel::CObjectKernel(CWorld& pWorld, const btScalar mass, const Ogre::Vector3& pos)
    : CObject("Kernel", pWorld, pos, mass)
    , CControlled("Kernel")
    , m_Front(Ogre::Vector3::UNIT_Z)
    , m_Gravity(btVector3(0.0f,0.0f,0.0f))
    , m_ActMove(Ogre::Vector3::ZERO)
    , m_Velocity(Ogre::Vector3::ZERO)
    , m_Direction(Ogre::Quaternion::IDENTITY)
    , m_SpeedMin(std::numeric_limits<Ogre::Real>::epsilon())
    , m_SpeedMax(10.0f)
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
    btVector3 new_gravity = m_pWorld->m_pGravityField->getObjectGravity(m_pBody->getBroadphaseProxy()->getUid());
    if( m_Gravity != new_gravity )
    {
        //m_Direction = BtOgre::Convert::toOgre(m_Gravity).getRotationTo(BtOgre::Convert::toOgre(new_gravity));
        m_Gravity = new_gravity;
        m_pBody->setGravity(new_gravity);
    }

    m_Front = CGame::getInstance()->m_pCamera->getDirection();
    //log_debug("Camera direction: x:%f y:%f z:%f", m_Front.x, m_Front.y, m_Front.z);
    //m_Front.y = 0;
    m_Front.normalise();
    m_Direction = Ogre::Vector3::NEGATIVE_UNIT_Y.getRotationTo(BtOgre::Convert::toOgre(m_Gravity)) * Ogre::Vector3::UNIT_Z.getRotationTo(m_Front);

    Ogre::Vector3 move = m_Direction * m_ActMove;

    if( ! move.isZeroLength() )
    {
        m_pBody->activate();
        m_Velocity += move.normalisedCopy() * m_SpeedMax * evt.timeSinceLastFrame * 10;
    }
    else
        m_Velocity -= m_Velocity * evt.timeSinceLastFrame * 10;

    // Processing action state
    if( m_Velocity.squaredLength() > (m_SpeedMax * m_SpeedMax) )
    {
        m_Velocity.normalise();
        m_Velocity *= m_SpeedMax;
    }
    else if( m_Velocity.squaredLength() < (m_SpeedMin * m_SpeedMin) )
        m_Velocity = Ogre::Vector3::ZERO;

    if( move.length() > 1.0 )
        m_Velocity *= move.normalisedCopy().length();
    else
        m_Velocity *= move.length();

    //log_debug("Cube force: F:%6.3f B:%6.3f L:%6.3f R:%6.3f", m_ForceForward, m_ForceBackward, m_ForceLeft, m_ForceRight);

    // Set rotation
    //m_pBody->setAngularVelocity(m_pBody->getAngularVelocity().rotate(m_pBody->getGravity(), 0.1f));
    m_pBody->setAngularVelocity(BtOgre::Convert::toBullet(m_Velocity).cross(m_Gravity.normalized()));

    // Direction vector
    ODD.drawLine(Ogre::Vector3::ZERO, m_Front * 10.0f, Ogre::ColourValue(1.0f, 0.0f, 1.0f));

    // Direction vector
    ODD.drawLine(m_pNode->getPosition(), (m_Direction * Ogre::Vector3::UNIT_Z)*10.0f + m_pNode->getPosition(), Ogre::ColourValue(0.0f, 1.0f, 1.0f));
    // Gravity vector (Down)
    ODD.drawLine(m_pNode->getPosition(), (m_Direction * Ogre::Vector3::UNIT_Y)*10.0f + m_pNode->getPosition(), Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    // Right vector (Down)
    ODD.drawLine(m_pNode->getPosition(), (m_Direction * Ogre::Vector3::NEGATIVE_UNIT_X)*10.0f + m_pNode->getPosition(), Ogre::ColourValue(1.0f, 1.0f, 0.0f));
    //ODD.drawLine(BtOgre::Convert::toOgre(m_pBody->getCenterOfMassPosition()), BtOgre::Convert::toOgre(m_pBody->getCenterOfMassPosition() + m_pBody->getGravity()*2), Ogre::ColourValue(1.0f, 0.0f, 1.0f));
    // ActMove vector
    ODD.drawLine(m_pNode->getPosition() + m_pEntity->getBoundingRadius(), m_ActMove*10.0f + m_pEntity->getBoundingRadius() + m_pNode->getPosition(), Ogre::ColourValue::Green);
    // Velocity vector
    ODD.drawLine(m_pNode->getPosition() + m_pEntity->getBoundingRadius(), m_Velocity*10.0f + m_pEntity->getBoundingRadius() + m_pNode->getPosition(), Ogre::ColourValue::Blue);
    // Bounding box
    Ogre::AxisAlignedBox cube = m_pEntity->getBoundingBox();
    cube.transform(m_pNode->_getFullTransform());
    ODD.drawCuboid(cube.getAllCorners(), Ogre::ColourValue::Red, true);
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
    switch(act){
    case 'f':
        m_ActMove.z = sig.value();
        break;
    case 'b':
        m_ActMove.z = -sig.value();
        break;
    case 'l':
        m_ActMove.x = sig.value();
        break;
    case 'r':
        m_ActMove.x = -sig.value();
        break;
    case 'j':
        m_ActMove.y = sig.value();
        break;
    }
}
