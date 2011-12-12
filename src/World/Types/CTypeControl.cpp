/**
 * @file    CTypeControl.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type control
 *
 *
 */

#include "World/Types/CTypeControl.h"

CTypeControl::CTypeControl()
    : CType("Control", "Controlling operations")
    , m_Control("Control", "Units to control", 1, 65535)
{
}
