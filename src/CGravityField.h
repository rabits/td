/**
 * @file    CGravityField.h
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

#ifndef CGRAVITYFIELD_H_INCLUDED
#define CGRAVITYFIELD_H_INCLUDED

#include "Common.h"

#include "OGRE/Ogre.h"
#include "World/CObject.h"
#include <BulletCollision/CollisionShapes/btBoxShape.h>

/** @brief Invisible box with gravity vector
 */
class CGravityElement
{
public:
    /** @brief Constructor of gravity element
     *
     * @param box
     * @param position
     * @param force
     *
     */
    CGravityElement(btVector3* box, btVector3* position, btVector3* force);

    /** @brief Destructor, cleaner of element
    */
    ~CGravityElement();

    btCollisionObject*            m_pGravityObj; ///< Bullet collision object
    btVector3*                    m_pForce; ///< Vector of gravity force

    int                           m_uid; ///< Unique id of element

    /** @brief Enumiration of status of element
     */
    enum ElementStatus
    {
        ES_ENABLED  = 1, ///< Element is enabled
        ES_DISABLED = 2  ///< Element is disabled
    };

private:
    ElementStatus                 m_status; ///< Current element status

    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     */
    CGravityElement(const CGravityElement& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CGravityElement& operator=(const CGravityElement& obj);
};

    /** @brief Global collection of invisible boxes with gravity vector
     */
class CGravityField
{
public:
    /** @brief Constructor of gravity field
     *
     * @param world
     * @param gravityValue
     *
     */
    CGravityField(CWorld* world, float gravityValue);

    /** @brief Destructor of field
     */
    ~CGravityField();


    // For world
    /** @brief Set gravity force of field
     *
     * @param newGravity
     * @return void
     *
     */
    void setGravityValue(float newGravity);

    /** @brief Get current gravity value
     *
     * @return float
     *
     */
    float getGravityValue();

    /** @brief Testing fields to contact with objects
     *
     * @return void
     *
     */
    void catchFieldContact();

    /** @brief Clearing all objects in field
     *
     * @return void clearObjectsInGravityField(){
     *
     */
    inline void clearObjectsInGravityField() { m_ObjectInGravityField.clear(); }


    // For elements
    /** @brief Add new gravity element to field
     *
     * @param el - Gravity element object
     * @return int
     *
     */
    int        add(CGravityElement* el);

    /** @brief Remove gravity element from field by id
     *
     * @param elId - Id of element
     * @return void
     *
     */
    void       remove(int elId);

    /** @brief Get gravity element object by id
     *
     * @param elId - Id of element
     * @return btVector3*
     *
     */
    btVector3* get(int elId);

    /** @brief Enabling gravity element by id
     *
     * @param elId - Id of element
     * @return void
     *
     */
    void       enable(int elId);

    /** @brief Disable gravity element by id
     *
     * @param elId - Id of element
     * @return void
     *
     */
    void       disable(int elId);

    // For objects
    /** @brief Zeroing of object gravity
     *
     * @param objectId
     * @param gravity
     * @return void
     *
     */
    void      zeroObjectGravity(int objectId, btVector3* gravity);

    /** @brief Set gravity to object
     *
     * @param objectId
     * @param gravity
     * @return void
     *
     */
    void      setObjectGravity(int objectId, btVector3* gravity);

    /** @brief Get object gravity
     *
     * @param objectId
     * @return btVector3
     *
     */
    btVector3 getObjectGravity(int objectId);

    struct SForceFieldCallback; ///< Callback structure
    static SForceFieldCallback   m_callbackResult; ///< Result of callback

private:
    std::map<int, bool>                         m_ObjectInGravityField; ///< Map causes object is in gravity field
    std::map<int, btVector3>                    m_ObjectGravityMap; ///< Objects gravity vector
    std::map<int, CGravityElement*>             m_GravityFieldMap; ///< Elements in field
    std::map<int, CGravityElement*>::iterator   m_oGravityFieldMap; ///< Current processing gravity element
    CWorld*                                     m_pWorld; ///< Linked world object

    float                                       m_GravityValue; ///< Force of gravity in field

    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     */
    CGravityField(const CGravityField& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CGravityField& operator=(const CGravityField& obj);
};

#endif // CGRAVITYFIELD_H_INCLUDED
