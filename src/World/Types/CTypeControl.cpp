
#include "World/Types/CTypeControl.h"

CTypeControl::CTypeControl()
    : CType("Control", "Controlling operations")
    , m_Control("Control", "Units to control", 1, 65535)
{
}
