
#ifndef CTYPESHIELD_H
#define CTYPESHIELD_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeShield
    : public CType
{
public:
    CTypeShield();

protected:
    CTypeParameter<std::string> m_Type;
    CTypeParameter<uint>        m_Absorption;

private:
};

#endif // CTYPESHIELD_H
