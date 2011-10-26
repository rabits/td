/**
 * @file    CControlled.cpp
 * @date    2010-10-07T15:55:38+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Controlled element
 *
 *
 */


#include "CControlled.h"

CControlled::CControlled()
    : m_vActions()
{
    //ctor
}

CControlled::~CControlled()
{
    //dtor
}

/** @brief Returning list of can be controlled actions
 *
 * @return std::vector<CAction>
 *
 */
std::vector<CAction> CControlled::getAvailableActions()
{
    return m_vActions;
}
