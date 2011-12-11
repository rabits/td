
#include "World/Types/CTypeShield.h"

CTypeShield::CTypeShield()
    : CType("Shield", "Defending system")
    , m_Type("Type", "Type of defending")
    , m_Absorption("Absorption", "", 0, 100)
{
    m_Type.addAvailable("Physical", "Reduces self health to defending unit");
    m_Type.addAvailable("Energy", "Creates force field");
    m_Type.addAvailable("Gravity", "Creates gravity field");
}
