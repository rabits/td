
#ifndef CTYPEENGINE_H
#define CTYPEENGINE_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeEngine
    : public CType
{
public:
    CTypeEngine();

protected:
    CTypeParameter<uint>        m_Power;

private:
};

#endif // CTYPEENGINE_H
