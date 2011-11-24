/**
 * @file    CSignal.cpp
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Input event
 *
 *
 */

#include "Nerv/CSignal.h"

CSignal::CSignal(unsigned int id, float value, float sens)
    : m_Id(id)
    , m_Value(value)
    , m_Sensitivity(sens)
{
}

CSignal::~CSignal()
{
}
