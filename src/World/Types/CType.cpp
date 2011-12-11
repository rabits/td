
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
