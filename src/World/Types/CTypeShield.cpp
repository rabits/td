/**
 * @file    CTypeShield.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type shield
 *
 *
 */

#include "World/Types/CTypeShield.h"

CTypeShield::CTypeShield()
    : CType("Shield", "Defending system")
    , m_Type("Type", "Type of defending")
    , m_Absorption("Absorption", "", 0, 100)
{
    m_Type.addAvailable("Physical", "Reduces self health to defending unit");
    m_Type.addAvailable("Energy", "Creates force field");
    m_Type.addAvailable("Gravity", "Creates gravity field");
}
