
#ifndef CTYPESPACE_H
#define CTYPESPACE_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeSpace
    : public CType
{
public:
    CTypeSpace();

protected:
    CTypeParameter<uint>        m_Space;

private:
};

#endif // CTYPESPACE_H
