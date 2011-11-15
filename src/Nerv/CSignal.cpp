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

CSignal::CSignal(unsigned int id, float value)
    : m_Id(id)
    , m_Value(value)
    , m_Type()
{
}

CSignal::~CSignal()
{
}

unsigned int CSignal::id()
{
    return m_Id;
}

float CSignal::value()
{
    return m_Value;
}

void CSignal::setSens(float sens)
{
    m_Value = std::min(m_Value * sens, 1.0f);
}

CSignal::Type CSignal::type()
{
    return m_Type;
}

void CSignal::type(CSignal::Type type)
{
    m_Type = type;
}
