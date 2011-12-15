/**
 * @file    CObject.h
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Base world object
 *
 *
 */

#ifndef COBJECT_H_INCLUDED
#define COBJECT_H_INCLUDED

#include "Common.h"

#include <OGRE/Ogre.h>

#include "btogre/BtOgreGP.h"
#include "btogre/BtOgrePG.h"

#include "CMaster.h"
#include "Nerv/CAction.h"

class CWorld;
class CGame;

/** @brief Father of all objects in game
 */
class CObject
    : public CMaster
{
public:
    /** @brief Constructor of object
     *
     * @param name
     * @param pWorld
     * @param pos
     * @param mass
     */
    CObject(const char* name, CWorld& pWorld, const Ogre::Vector3& pos = Ogre::Vector3(), const btScalar mass = 0.0);

    /** @brief Destructor of object
     */
    virtual ~CObject();


    /** @brief Setting object parent
     *
     * @param pParent
     * @return void
     *
     */
    void setParent(CObject* pParent){ m_pParent = pParent; }

    /** @brief Setting object World
     *
     * @param pWorld
     * @return void
     *
     */
    void setWorld(CWorld* pWorld){ m_pWorld = pWorld; }


    /** @brief Delete all children objects
     *
     * @return void
     *
     */
    void clearChildrens();

    /** @brief Add child object
     *
     * @param pChild
     * @return void
     *
     */
    void attachChild(CObject* pChild);

    /** @brief Return childrens list
     *
     * @return std::vector<CObject*>*
     *
     */
    std::vector<CObject*>* getChildrens();

    /** @brief Gets scene node of current object
     *
     * @return Ogre::Node*
     */
    Ogre::SceneNode* node(){ return m_pNode; }

    /** @brief Groups for collision detection
     */
    enum CollisionObjectGroup {
        STATIC_OBJECT       = 0x1,   ///< Static object 00000001
        DYNAMIC_OBJECT      = 0x2,   ///< Dynamic object 00000010
        FIELD_OBJECT        = 0x4    ///< Field object 00000100
    };

protected:
    Ogre::SceneNode*                     m_pNode; ///< Object scene node

    bool                                 m_HasChild; ///< Object is has any child
    std::vector<CObject*>                m_Childrens; ///< List of child objects
    std::vector<CObject*>::iterator      m_itChildrens; ///< Current processing child object

    CObject*                             m_pParent; ///< Pointer to parent object
    CGame*                               m_pGame;   ///< Pointer to Game Object
    CWorld*                              m_pWorld;  ///< Pointer to World

    Ogre::Vector3                        m_Position; ///< Object position (position may be changed by bullet and user)

    Ogre::Entity*                        m_pEntity;  ///< Ogre entity

    btRigidBody*                         m_pBody;    ///< Physics rigid body
    btCollisionShape*                    m_pShape;   ///< Shape of collision
    btScalar                             m_Mass;     ///< Mass of object
    BtOgre::RigidBodyState*              m_pState;   ///< Rigid body state

private:
    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     */
    CObject(const CObject& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CObject& operator=(const CObject& obj);
};


#endif // COBJECT_H_INCLUDED
