/**
 * @file    CTypeSpace.h
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

#ifndef CTYPESPACE_H
#define CTYPESPACE_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeSpace
    : public CType
{
public:
    CTypeSpace();

protected:
    CTypeParameter<uint>        m_Space;

private:
};

#endif // CTYPESPACE_H
