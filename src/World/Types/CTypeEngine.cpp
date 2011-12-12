/**
 * @file    CTypeEngine.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type engine
 *
 *
 */

#include "World/Types/CTypeEngine.h"

CTypeEngine::CTypeEngine()
    : CType("Engine", "Mechanical impact")
    , m_Power("Power", "", 1, 65535)
{
}
