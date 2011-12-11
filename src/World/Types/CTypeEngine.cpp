
#include "World/Types/CTypeEngine.h"

CTypeEngine::CTypeEngine()
    : CType("Engine", "Mechanical impact")
    , m_Power("Power", "", 1, 65535)
{
}
