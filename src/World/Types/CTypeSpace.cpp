/**
 * @file    CTypeSpace.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type space
 *
 *
 */

#include "World/Types/CTypeSpace.h"

CTypeSpace::CTypeSpace()
    : CType("Space", "Physical space in hold")
    , m_Space("Space", "", 1, 65535)
{
}
