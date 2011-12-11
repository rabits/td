
#ifndef CTYPEWAVERECIEVER_H
#define CTYPEWAVERECIEVER_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeWaveReciever
    : public CType
{
public:
    CTypeWaveReciever();

protected:
    CTypeParameter<uint>        m_Power;
    CTypeParameter<std::string> m_Type;
    CTypeParameter<std::string> m_Spectrum;
    CTypeParameter<uint>        m_NoiceReduction;

private:
};

#endif // CTYPEWAVEEMITTER_H
