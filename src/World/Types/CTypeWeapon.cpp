/**
 * @file    CTypeWeapon.cpp
 * @date    2011-12-12T18:27:42+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type weapon
 *
 *
 */

#include "World/Types/CTypeWeapon.h"

CTypeWeapon::CTypeWeapon()
    : CType("Weapon", "Damaging")
    , m_Type("Type", "")
    , m_Range("Range", "", 1, 65535)
{
    m_Type.addAvailable("Contact", "Near field weapon like knife");
    m_Type.addAvailable("Firearm", "Physical damage by contact with shell");
    m_Type.addAvailable("Granade", "Granade launcher");
    m_Type.addAvailable("Laser", "Energetic or heat damage");
    m_Type.addAvailable("Gravity", "");
}
