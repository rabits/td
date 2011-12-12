/**
 * @file    CTypeEnergy.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type energy
 *
 *
 */

#include "World/Types/CTypeEnergy.h"

CTypeEnergy::CTypeEnergy()
    : CType("Energy", "Resource for engines or devices")
    , m_Energy("Energy", "Maximum of containing energy", 1.0f, std::numeric_limits<float>::max())
    , m_Type("Type", "Substance of energy containing")
    , m_Regenerate("Regenerate", "Speed of energy regeneration (per second)", 0.0f, std::numeric_limits<float>::max())
{
    m_Type.addAvailable("Gasoline", "Organic fuel based on petroleum");
    m_Type.addAvailable("Electricity", "Flow of electrons");
    m_Type.addAvailable("Plasma", "Ionized gas");
    m_Type.addAvailable("Dark Energy", "From universe deep");
}
