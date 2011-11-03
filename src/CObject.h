/**
 * @file    CObject.h
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Base object
 *
 *
 */

#ifndef COBJECT_H_INCLUDED
#define COBJECT_H_INCLUDED

#include "Common.h"

#include <string>
#include <vector>
#include <OGRE/Ogre.h>

#include "btogre/BtOgreGP.h"
#include "btogre/BtOgrePG.h"

#include "CAction.h"

class CObjectWorld;
class CGame;

/** @brief Father of all objects in game
 */
class CObject
{
public:
    /** @brief Constructor of object
     */
    CObject();

    /** @brief Copy constructor of object
     *
     * @param obj const CObject&
     *
     */
    CObject(const CObject &obj);

    /** @brief Destructor of object
     */
    virtual ~CObject();


    /** @brief Setting object parent
     *
     * @param pParent CObject*
     * @return void
     *
     */
    void setParent(CObject* pParent);

    /** @brief Setting object World
     *
     * @param pWorld CObjectWorld*
     * @return void
     *
     */
    void setWorld(CObjectWorld* pWorld);


    /** @brief Delete all children objects
     *
     * @return void
     *
     */
    void clearChildrens();

    /** @brief Delete all actions of object
     *
     * @return void
     *
     */
    void clearActions();

    /** @brief Add child object
     *
     * @param pChild CObject*
     * @return void
     *
     */
    void attachChild(CObject* pChild);

    /** @brief Add object action
     *
     * @param pAction CAction*
     * @return void
     *
     */
    void attachAction(CAction* pAction);

    /** @brief Return childrens list
     *
     * @return std::vector<CObject*>*
     *
     */
    std::vector<CObject*> *getChildrens();

    /** @brief Return list of actions
     *
     * @return std::vector<CAction*>*
     *
     */
    std::vector<CAction*> *getActions();

    //--------------------------------------------------------------------------------------
    // Pure virtual functions. Must be overriden in derived objects.
    //--------------------------------------------------------------------------------------

    /** @brief Update object data, it may be animation or just object translation or so on
     *
     * @return virtual void
     *
     */
    virtual void update() = 0;

    /** @brief Initialize object
     *
     * @return virtual void
     *
     */
    virtual void init() = 0;

    /** @brief Setting up object state
     *
     * @param State int
     * @return virtual void
     *
     */
    virtual void setObjectState(int State) = 0;

    Ogre::SceneNode                     *m_pNode; ///< Object scene node

    /** @brief Groups for collision detection
     */
    enum CollisionObjectGroup {
        STATIC_OBJECT       = 0x1,   ///< Static object 00000001
        DYNAMIC_OBJECT      = 0x2,   ///< Dynamic object 00000010
        FIELD_OBJECT        = 0x4    ///< Field object 00000100
    };

protected:
    bool                                 m_HasChild; ///< Object is has any child
    std::string                          m_ObjectName; ///< Object name (for debug purpose and for scripts)

    std::vector<CObject*>                m_Childrens; ///< List of child objects
    std::vector<CObject*>::iterator      m_itChildrens; ///< Current processing child object

    std::vector<CAction*>                m_Actions; ///< List of this object actions

    CObject                             *m_pParent; ///< Pointer to parent object
    CGame                               *m_pGame;   ///< Pointer to Game Object
    CObjectWorld                        *m_pWorld;  ///< Pointer to World

    Ogre::Vector3                        m_Position; ///< Object position (position may be changed by bullet and user)

    Ogre::Entity                        *m_pEntity;  ///< Ogre entity

    btRigidBody                         *m_pBody;    ///< Physics rigid body
    btCollisionShape                    *m_pShape;   ///< Shape of collision
    btScalar                             m_Mass;     ///< Mass of object
    BtOgre::RigidBodyState              *m_pState;   ///< Rigid body state
};


#endif // COBJECT_H_INCLUDED
