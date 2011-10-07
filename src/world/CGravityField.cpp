/** // CGRAVITYFIELD_CPP
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
 */

#include "world/CGravityField.h"
#include "world/CObjectWorld.h"
#include "app/CGame.h"

CGravityElement::CGravityElement(btVector3* box, btVector3* position, btVector3* force)
{
    m_pGravityObj = new btCollisionObject();
    m_pGravityObj->setCollisionShape(new btBoxShape(*box));
    m_pGravityObj->getWorldTransform().setOrigin(*position);
    m_pForce = force;
    m_status = CGravityElement::ES_ENABLED;
}

CGravityElement::~CGravityElement()
{
}

//-----------------------------

CGravityField::CGravityField(CObjectWorld *world, float gravityValue)
{
    m_pWorld = world;
    m_gravityValue = gravityValue;
    m_gravityFieldMap.clear();
    m_objectGravityMap.clear();
}

CGravityField::~CGravityField()
{
}

//-----------------------------

void CGravityField::setGravityValue(float newGravity)
{
    m_gravityValue = newGravity;
}

float CGravityField::getGravityValue()
{
    return m_gravityValue;
}

struct CGravityField::SForceFieldCallback : public btCollisionWorld::ContactResultCallback
{
    virtual    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1)
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
            Ogre::LogManager::getSingletonPtr()->logMessage("Connected to GravityField object is not regid body: "+Ogre::StringConverter::toString(colObj1->getBroadphaseHandle()->getUid()));

        return 0;
    }
} CGravityField::m_callbackResult;

void CGravityField::catchFieldContact()
{
    for ( m_itGravityFieldMap = m_gravityFieldMap.begin() ; m_itGravityFieldMap != m_gravityFieldMap.end(); m_itGravityFieldMap++ )
        m_pWorld->phyWorld->contactTest((*m_itGravityFieldMap).second->m_pGravityObj, m_callbackResult);
}

//-----------------------------

int CGravityField::add(CGravityElement *el)
{
    m_pWorld->phyWorld->addCollisionObject(el->m_pGravityObj, CObject::FIELD_OBJECT, CObject::DYNAMIC_OBJECT);
    el->m_uid = el->m_pGravityObj->getBroadphaseHandle()->getUid();
    m_gravityFieldMap[el->m_uid] = el;

    return el->m_uid;
}

void CGravityField::remove(int elId)
{
    m_pWorld->phyWorld->removeCollisionObject(m_gravityFieldMap[elId]->m_pGravityObj);
    m_gravityFieldMap.erase(elId);
}

btVector3* CGravityField::get(int elId)
{
    if( m_gravityFieldMap.find(elId) != m_gravityFieldMap.end() )
        return m_gravityFieldMap[elId]->m_pForce;

    Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Not found Gravity Field with id#"+Ogre::StringConverter::toString(elId));
    return NULL;
}

void CGravityField::enable(int elId)
{
}

void CGravityField::disable(int elId)
{
}

//-----------------------------

void CGravityField::setObjectGravity(int objectId, btVector3* gravity)
{
    // Set the object located in a gravitational field
    m_objectInGravityField[objectId] = true;

    // Change Gravity Vector
    m_objectGravityMap[objectId] = m_objectGravityMap[objectId] + (*gravity);
}

void CGravityField::zeroObjectGravity(int objectId, btVector3* gravity)
{
    m_objectGravityMap[objectId] = (*gravity);
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
