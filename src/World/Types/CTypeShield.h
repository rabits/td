/**
 * @file    CTypeShield.h
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

#ifndef CTYPESHIELD_H
#define CTYPESHIELD_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeShield
    : public CType
{
public:
    CTypeShield();

protected:
    CTypeParameter<std::string> m_Type;
    CTypeParameter<uint>        m_Absorption;

private:
};

#endif // CTYPESHIELD_H
