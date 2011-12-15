/**
 * @file    CEye.cpp
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
#include "CEye.h"

#include "CGame.h"

CEye::CEye(Ogre::Camera* cam)
    : CControlled("CEye")
    , CTypeCamera()
{
    camera(cam);
}

CEye::~CEye()
{
}

void CEye::update(const Ogre::Real time_since_last_frame)
{
    CTypeCamera::update(time_since_last_frame);
}
