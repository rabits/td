
#ifndef CTYPEENERGY_H
#define CTYPEENERGY_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeEnergy
    : public CType
{
public:
    CTypeEnergy();

protected:
    CTypeParameter<float>        m_Energy;
    CTypeParameter<std::string>  m_Type;
    CTypeParameter<float>        m_Regenerate;

private:
};

#endif // CTYPEENERGY_H
