/**
 * @file    CGravityField.cpp
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Gravity field
 *
 *
 */


#include "CGravityField.h"
#include "CObjectWorld.h"
#include "CGame.h"

CGravityElement::CGravityElement(btVector3* box, btVector3* position, btVector3* force)
    : m_pGravityObj(NULL)
    , m_pForce(force)
    , m_uid(0)
    , m_status(ES_ENABLED)
{
    m_pGravityObj = new btCollisionObject();
    m_pGravityObj->setCollisionShape(new btBoxShape(*box));
    m_pGravityObj->getWorldTransform().setOrigin(*position);
}

CGravityElement::~CGravityElement()
{
    delete m_pGravityObj;
}


/** @brief Struct for auto-detecting objects in field
 */
struct CGravityField::SForceFieldCallback : public btCollisionWorld::ContactResultCallback
{
    /** @brief Bullet auto-execute this function if object contacts with field
     *
     * @param cp btManifoldPoint&
     * @param colObj0 const btCollisionObject*
     * @param partId0 int
     * @param index0 int
     * @param colObj1 const btCollisionObject*
     * @param partId1 int
     * @param index1 int
     * @return virtual btScalar
     *
     */
    virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1)
    {
        if (colObj1->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            CGravityField* gf = (*CGame::getInstance()->o_currentWorld)->m_pGravityField;
            gf->setObjectGravity(
                colObj1->getBroadphaseHandle()->getUid(),
                gf->get(colObj0->getBroadphaseHandle()->getUid())
            );
        }
        else
            log_debug("Connected to GravityField object is not regid body uid#%d", colObj1->getBroadphaseHandle()->getUid());

        return 0;
    }
} CGravityField::m_callbackResult; ///< Callback object for Bullet


CGravityField::CGravityField(CObjectWorld *world, float gravityValue)
    : m_objectInGravityField()
    , m_objectGravityMap()
    , m_gravityFieldMap()
    , m_itGravityFieldMap(NULL)
    , m_pWorld(world)
    , m_gravityValue(gravityValue)
{
    m_gravityFieldMap.clear();
    m_objectGravityMap.clear();
}

CGravityField::~CGravityField()
{
}

void CGravityField::setGravityValue(float newGravity)
{
    m_gravityValue = newGravity;
}

float CGravityField::getGravityValue()
{
    return m_gravityValue;
}

void CGravityField::catchFieldContact()
{
    for ( m_itGravityFieldMap = m_gravityFieldMap.begin() ; m_itGravityFieldMap != m_gravityFieldMap.end(); m_itGravityFieldMap++ )
        m_pWorld->m_pPhyWorld->contactTest((*m_itGravityFieldMap).second->m_pGravityObj, m_callbackResult);
}

int CGravityField::add(CGravityElement *el)
{
    m_pWorld->m_pPhyWorld->addCollisionObject(el->m_pGravityObj, CObject::FIELD_OBJECT, CObject::DYNAMIC_OBJECT);
    el->m_uid = el->m_pGravityObj->getBroadphaseHandle()->getUid();
    m_gravityFieldMap[el->m_uid] = el;

    return el->m_uid;
}

void CGravityField::remove(int elId)
{
    m_pWorld->m_pPhyWorld->removeCollisionObject(m_gravityFieldMap[elId]->m_pGravityObj);
    m_gravityFieldMap.erase(elId);
}

btVector3* CGravityField::get(int elId)
{
    if( m_gravityFieldMap.find(elId) != m_gravityFieldMap.end() )
        return m_gravityFieldMap[elId]->m_pForce;

    log_error("Not found Gravity Field with id#%d", elId);
    return NULL;
}

void CGravityField::enable(int elId)
{
}

void CGravityField::disable(int elId)
{
}

void CGravityField::zeroObjectGravity(int objectId, btVector3* gravity)
{
    m_objectGravityMap[objectId] = (*gravity);
}

void CGravityField::setObjectGravity(int objectId, btVector3* gravity)
{
    // Set the object located in a gravitational field
    m_objectInGravityField[objectId] = true;

    // Change Gravity Vector
    m_objectGravityMap[objectId] = m_objectGravityMap[objectId] + (*gravity);
}

btVector3 CGravityField::getObjectGravity(int objectId)
{
    // Zeroficate gravity
    btVector3 temp = m_objectGravityMap[objectId];
    m_objectGravityMap[objectId].setZero();

    if( temp.length() != 0 )
        temp.normalize();

    return temp * m_gravityValue;
};
