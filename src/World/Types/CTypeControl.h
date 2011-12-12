/**
 * @file    CTypeControl.h
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

#ifndef CTYPECONTROL_H
#define CTYPECONTROL_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeControl
    : public CType
{
public:
    CTypeControl();

protected:
    CTypeParameter<uint>        m_Control;

private:
};

#endif // CTYPECONTROL_H
