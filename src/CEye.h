/**
 * @file    CEye.h
 * @date    2011-10-24T23:26:46+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Container for camera
 *
 *
 */
#ifndef CEYE_H
#define CEYE_H

#include "Common.h"

#include "World/Types/CTypeCamera.h"

/** @brief It is invisible camera controller for game needs
 *
 * Container for camera for connection to controll set
 */
class CEye
    : public CTypeCamera
{
public:
    /** @brief Constructor
     */
    CEye(Ogre::Camera* cam);

    /** @brief Destructor
     */
    virtual ~CEye();

    /** @brief Update eye state
     *
     * @param time_since_last_frame
     * @return void
     *
     */
    void update(const Ogre::Real time_since_last_frame);

protected:

private:
    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     *
     */
    CEye(const CEye& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CEye& operator=(const CEye& obj);
};

#endif // CEYE_H
