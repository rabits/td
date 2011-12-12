/**
 * @file    CType.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Object type
 *
 *
 */

#include "CType.h"

template <class Type>
void CType::CTypeParameter<Type>::value(const Type& value)
{
    switch( m_Limit ) {
    case CTypeParameter::LIMIT_NO:
        m_Value = value;
        break;
    case CTypeParameter::LIMIT_MINMAX:
        if( (value >= m_ValueMin) && (value <= m_ValueMax) )
            m_Value = value;
        break;
    case CTypeParameter::LIMIT_AVAILABLE:
        if( m_ValuesAvailable[value] != m_ValuesAvailable.end() )
            m_Value = value;
        break;
    }
}
