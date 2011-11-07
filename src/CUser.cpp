/**
 * @file    CUser.cpp
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

#include "CUser.h"

CUser::CUser()
    : CData("User")
    , m_name("")
    , m_pControlledObject(NULL)
{
}

CUser::~CUser()
{
}

void CUser::setControlledObject(CControlled* obj)
{
    m_pControlledObject = obj;
}

void CUser::setWorld(CWorld* world)
{
}

void CUser::update(const Ogre::FrameEvent& evt)
{
}
