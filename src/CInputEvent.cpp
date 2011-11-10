/**
 * @file    CInputEvent.cpp
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

#include "CInputEvent.h"

CInputEvent::CInputEvent(int id, float value)
    : m_Id(id)
    , m_Value(value)
{
}

CInputEvent::~CInputEvent()
{
}

int CInputEvent::id()
{
    return m_Id;
}

float CInputEvent::value()
{
    return m_Value;
}

void CInputEvent::setSens(float sens)
{
    m_Value = std::min(m_Value * sens, 1.0f);
}

CInputEvent::Type CInputEvent::type()
{
    return m_Type;
}

void CInputEvent::type(CInputEvent::Type type)
{
    m_Type = type;
}
