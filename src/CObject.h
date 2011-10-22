/**
 * @file    CObject.h
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   Base object
 *
 *
 */


#ifndef COBJECT_H_INCLUDED
#define COBJECT_H_INCLUDED

#include "config.h"

#include <string>
#include <vector>
#include <OGRE/Ogre.h>

#include "btogre/BtOgreGP.h"
#include "btogre/BtOgrePG.h"

class CObjectWorld;
class CGame;

class CObject
{
public:
    // Vars
    // Object has any child
    bool                m_bHasChild;
    // Object name (for debug purpose and for scripts)
    std::string            m_sObjectName;

    // Functions
    CObject();
    ~CObject();

    // Delete all objects children to aviod memory leaks.
    void clearChildrenList();
    // Setting object parent
    void setParent(CObject* pParent);
    // Setting Game pointer
    void setGame(CGame* pGame);
    // Setting object World
    void setWorld(CObjectWorld* pWorld);
    // Add child object
    void attachChild(CObject* pChild);
    // Return children list for the recursive scene traversal
    std::vector<CObject*> *getChildrenList();

    //--------------------------------------------------------------------------------------
    // Pure virtual functions. Must be overriden in derived objects.
    //--------------------------------------------------------------------------------------
    // Update object data, it may be animation or just object translation or so on.
    virtual void update() = 0;
    // Initialize object
    virtual void init() = 0;
    // Seeting up object state.
    virtual void setObjectState(int State) = 0;

    Ogre::SceneNode                        *m_pNode;

    // Groups for collision detection
    enum CollisionObjectGroup {
        STATIC_OBJECT        = 0x1,    //00000001
        DYNAMIC_OBJECT        = 0x2,    //00000010
        FIELD_OBJECT        = 0x4    //00000100
    };

protected:
    // List of child objects
    std::vector<CObject*>                m_ChildrenList;
    std::vector<CObject*>::iterator        m_itChildrenList;

    // Pointer to parent object
    CObject                                *m_pParent;
    // Pointer to Game Object
    CGame                                *m_pGame;
    // Pointer to World
    CObjectWorld                        *m_pWorld;

    // Object start position (position may be changed by bullet)
    Ogre::Vector3                        m_position;

    Ogre::Entity                        *m_pEntity;

    btRigidBody                         *m_pBody;
    btCollisionShape                     *m_pShape;
    btScalar                             m_mass;

    BtOgre::RigidBodyState                *m_pState;
};


#endif // COBJECT_H_INCLUDED
