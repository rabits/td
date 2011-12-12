/**
 * @file    CTypeRadar.cpp
 * @date    2011-12-12T18:27:42+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type radar
 *
 *
 */

#include "World/Types/CTypeRadar.h"

CTypeRadar::CTypeRadar()
    : CType("Radar", "Displays units on interactive map")
    , m_Type("Type", "")
    , m_Range("Range", "", 1, 65535)
{
    m_Type.addAvailable("Simple", "Only units");
    m_Type.addAvailable("IFF", "Show units with identification system");
    m_Type.addAvailable("Map", "Only visible map");
    m_Type.addAvailable("Memory Map", "Only visible map with memory");
    m_Type.addAvailable("Units Map", "Map with units");
    m_Type.addAvailable("IFF Map", "Map with unit identification friend or foe");
}
