/**
 * @file    CObjectCube.cpp
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Cube object
 *
 *
 */

#include "CObjectCube.h"
#include "CGravityField.h"
#include "CGame.h"

CObjectCube::CObjectCube(CObjectWorld &pWorld, CObjectCube::Cube_Size size, const Ogre::Vector3 &pos)
    : m_cubeSize(size)
{
    if( &pWorld != NULL )
        setWorld(&pWorld);

    m_Position = pos;
}

void CObjectCube::init()
{
    if( m_pParent != NULL )
    {
        //Create Ogre stuff.
        m_pEntity = m_pGame->m_pSceneMgr->createEntity("objectcube.mesh");
        m_pNode = m_pParent->m_pNode->createChildSceneNode(m_Position);
        m_pNode->attachObject(m_pEntity);
        m_pNode->scale(Ogre::Vector3(m_cubeSize));

        //Create shape.
        BtOgre::StaticMeshToShapeConverter converter(m_pEntity);
        m_pShape = converter.createBox();
        m_pShape->setLocalScaling(btVector3(m_cubeSize, m_cubeSize, m_cubeSize));

        //Calculate inertia.
        m_Mass = 0;
        btVector3 inertia;
        m_pShape->calculateLocalInertia(m_Mass, inertia);

        //Create BtOgre MotionState (connects Ogre and Bullet).
        m_pState = new BtOgre::RigidBodyState(m_pNode);

        //Create the Body.
        m_pBody = new btRigidBody(m_Mass, m_pState, m_pShape, inertia);
        m_pWorld->m_pPhyWorld->addRigidBody(m_pBody, CObject::STATIC_OBJECT, CObject::DYNAMIC_OBJECT);

        // Get size of cube
        Ogre::Vector3 size = converter.getSize()*m_cubeSize;

        float wc = 0.75f;        // Width coefficient
        float hc = 0.125f;        // Height coefficient
        float pc = 0.625f;        // Position coefficient

        // Create Force Field around cube
        // +Y (Up)
        m_gravityVolumes[0] = m_pWorld->m_pGravityField->add(new CGravityElement(new btVector3(size.x*wc, size.y*hc, size.z*wc), new btVector3(m_Position.x, m_Position.y+size.y*pc, m_Position.z), new btVector3(0.0f, -1.0f, 0.0f)));
        // -Y (Down)
        m_gravityVolumes[1] = m_pWorld->m_pGravityField->add(new CGravityElement(new btVector3(size.x*wc, size.y*hc, size.z*wc), new btVector3(m_Position.x, m_Position.y-size.y*pc, m_Position.z), new btVector3(0.0f, 1.0f, 0.0f)));
        // +X (Right)
        m_gravityVolumes[2] = m_pWorld->m_pGravityField->add(new CGravityElement(new btVector3(size.x*hc, size.y*wc, size.z*wc), new btVector3(m_Position.x+size.x*pc, m_Position.y, m_Position.z), new btVector3(-1.0f, 0.0f, 0.0f)));
        // -X (Left)
        m_gravityVolumes[3] = m_pWorld->m_pGravityField->add(new CGravityElement(new btVector3(size.x*hc, size.y*wc, size.z*wc), new btVector3(m_Position.x-size.x*pc, m_Position.y, m_Position.z), new btVector3(1.0f, 0.0f, 0.0f)));
        // +Z (Front)
        m_gravityVolumes[4] = m_pWorld->m_pGravityField->add(new CGravityElement(new btVector3(size.x*wc, size.y*wc, size.z*hc), new btVector3(m_Position.x, m_Position.y, m_Position.z+size.z*pc), new btVector3(0.0f, 0.0f, -1.0f)));
        // -Z (Back)
        m_gravityVolumes[5] = m_pWorld->m_pGravityField->add(new CGravityElement(new btVector3(size.x*wc, size.y*wc, size.z*hc), new btVector3(m_Position.x, m_Position.y, m_Position.z-size.z*pc), new btVector3(0.0f, 0.0f, 1.0f)));
    }
}

CObjectCube::~CObjectCube()
{
    for(int i=0; i<6; i++)
        m_pWorld->m_pGravityField->remove(i);
}

void CObjectCube::update()
{
}

void CObjectCube::setObjectState(int iState)
{
}

void CObjectCube::setCubeSize(CObjectCube::Cube_Size cubeSize)
{
    m_cubeSize = cubeSize;
}
