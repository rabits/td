
#ifndef CTYPEWAVEEMITTER_H
#define CTYPEWAVEEMITTER_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeWaveEmitter
    : public CType
{
public:
    CTypeWaveEmitter();

protected:
    CTypeParameter<uint>        m_Power;
    CTypeParameter<std::string> m_Type;
    CTypeParameter<std::string> m_Directivity;
    CTypeParameter<std::string> m_Spectrum;

private:
};

#endif // CTYPEWAVEEMITTER_H
