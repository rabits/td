/**
 * @file    CTypeWeapon.h
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

#ifndef CTYPEWEAPON_H
#define CTYPEWEAPON_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeWeapon
    : public CType
{
public:
    CTypeWeapon();

protected:
    CTypeParameter<std::string> m_Type;
    CTypeParameter<uint>        m_Range;

private:
};

#endif // CTYPEWEAPON_H
