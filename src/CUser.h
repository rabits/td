/**
 * @file    CUser.h
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   User
 *
 *
 */

#ifndef CUSER_H
#define CUSER_H

#include "config.h"

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include "CControlled.h"
#include "CObjectWorld.h"

/** @brief Self user
 *
 * Can control objects
 */
class CUser
    : public CControlled
{
public:
    /** @brief Constructor of user
     */
    CUser();

    /** @brief Destructor of user
     */
    ~CUser();

    /** @brief Set of object, which will be controlled
     *
     * @param obj CControlled*
     * @return void
     *
     */
    void setControlledObject(CControlled *obj);

    /** @brief Set world parent
     *
     * @param world CObjectWorld*
     * @return void
     *
     */
    void setWorld(CObjectWorld *world);

protected:
    Ogre::String                 m_name; ///< User name
    CControlled                 *m_pControlledObject; ///< Object under control

private:
};

#endif // CUSER_H
