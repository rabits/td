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
#include "World/CWorld.h"
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
     * @param cp
     * @param colObj0
     * @param partId0
     * @param index0
     * @param colObj1
     * @param partId1
     * @param index1
     * @return virtual btScalar
     *
     */
    virtual btScalar addSingleResult(btManifoldPoint&, const btCollisionObject* colObj0, int, int, const btCollisionObject* colObj1, int, int)
    {
        if (colObj1->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            CGravityField* gf = (*CGame::getInstance()->m_oCurrentWorld)->m_pGravityField;
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


CGravityField::CGravityField(CWorld* world, float gravityValue)
    : m_ObjectInGravityField()
    , m_ObjectGravityMap()
    , m_GravityFieldMap()
    , m_oGravityFieldMap(NULL)
    , m_pWorld(world)
    , m_GravityValue(gravityValue)
{
}

CGravityField::~CGravityField()
{
}

void CGravityField::setGravityValue(float newGravity)
{
    m_GravityValue = newGravity;
}

float CGravityField::getGravityValue()
{
    return m_GravityValue;
}

void CGravityField::catchFieldContact()
{
    for ( m_oGravityFieldMap = m_GravityFieldMap.begin() ; m_oGravityFieldMap != m_GravityFieldMap.end(); m_oGravityFieldMap++ )
        m_pWorld->m_pPhyWorld->contactTest((*m_oGravityFieldMap).second->m_pGravityObj, m_callbackResult);
}

int CGravityField::add(CGravityElement* el)
{
    m_pWorld->m_pPhyWorld->addCollisionObject(el->m_pGravityObj, CObject::FIELD_OBJECT, CObject::DYNAMIC_OBJECT);
    el->m_uid = el->m_pGravityObj->getBroadphaseHandle()->getUid();
    m_GravityFieldMap[el->m_uid] = el;

    return el->m_uid;
}

void CGravityField::remove(int elId)
{
    // @todo fix this remove sigfault
    m_pWorld->m_pPhyWorld->removeCollisionObject(m_GravityFieldMap[elId]->m_pGravityObj);
    m_GravityFieldMap.erase(elId);
}

btVector3* CGravityField::get(int elId)
{
    if( m_GravityFieldMap.find(elId) != m_GravityFieldMap.end() )
        return m_GravityFieldMap[elId]->m_pForce;

    log_error("Not found Gravity Field with id#%d", elId);
    return NULL;
}

void CGravityField::enable(int elId)
{
    // @todo create this function
}

void CGravityField::disable(int elId)
{
    // @todo create this function
}

void CGravityField::zeroObjectGravity(int objectId, btVector3* gravity)
{
    m_ObjectGravityMap[objectId] = (*gravity);
}

void CGravityField::setObjectGravity(int objectId, btVector3* gravity)
{
    // Set the object located in a gravitational field
    m_ObjectInGravityField[objectId] = true;

    // Change Gravity Vector
    m_ObjectGravityMap[objectId] = m_ObjectGravityMap[objectId] + (*gravity);
}

btVector3 CGravityField::getObjectGravity(int objectId)
{
    // Zeroficate gravity
    btVector3 temp = m_ObjectGravityMap[objectId];
    m_ObjectGravityMap[objectId].setZero();

    if( temp.length() != 0 )
        temp.normalize();

    return temp * m_GravityValue;
};
