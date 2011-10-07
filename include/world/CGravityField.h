/** // CGRAVITYFIELD_H
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
 */

#ifndef CGRAVITYFIELD_H_INCLUDED
#define CGRAVITYFIELD_H_INCLUDED

#include "config.h"

#include "OGRE/Ogre.h"
#include "object/CObject.h"
#include <map>
#include <BulletCollision/CollisionShapes/btBoxShape.h>

class CGravityElement
{
public:
    CGravityElement(btVector3* box, btVector3* position, btVector3* force);
    ~CGravityElement();

    btCollisionObject            *m_pGravityObj;
    btVector3                    *m_pForce;

    int                            m_uid;

    enum ElementStatus
    {
        ES_ENABLED = 1,
        ES_DISABLED = 2
    };

private:
    ElementStatus                m_status;
};

class CGravityField
{
private:
    struct                                        SForceFieldCallback;

public:
    CGravityField(CObjectWorld *world, float gravityValue);
    ~CGravityField();

    // For world
    void setGravityValue(float newGravity);
    float getGravityValue();
    void catchFieldContact();
    inline void clearObjectInGravityField() { m_objectInGravityField.clear(); }

    // For elements
    int add(CGravityElement *el);
    void remove(int elId);
    btVector3* get(int elId);
    void enable(int elId);
    void disable(int elId);

    // For objects
    void zeroObjectGravity(int objectId, btVector3* gravity);
    void setObjectGravity(int objectId, btVector3* gravity);
    btVector3 getObjectGravity(int objectId);

    // Callback
    static CGravityField::SForceFieldCallback    m_callbackResult;

private:
    std::map<int, bool>                            m_objectInGravityField;
    std::map<int, btVector3>                    m_objectGravityMap;
    std::map<int, CGravityElement*>                m_gravityFieldMap;
    std::map<int, CGravityElement*>::iterator    m_itGravityFieldMap;
    CObjectWorld                                *m_pWorld;

    float                                        m_gravityValue;
};

#endif // CGRAVITYFIELD_H_INCLUDED
