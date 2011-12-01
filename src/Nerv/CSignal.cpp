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

CSignal::CSignal()
    : m_Id(0)
    , m_Value(0.0f)
    , m_Sensitivity(0.0f)
    , m_Limit(0.0f)
{
}

CSignal::CSignal(unsigned int id, float value, float sens, float limit)
    : m_Id(id)
    , m_Value(value)
    , m_Sensitivity(sens)
    , m_Limit(limit)
{
}

CSignal::~CSignal()
{
}
