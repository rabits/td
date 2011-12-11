
#ifndef CTYPECONTROL_H
#define CTYPECONTROL_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeControl
    : public CType
{
public:
    CTypeControl();

protected:
    CTypeParameter<uint>        m_Control;

private:
};

#endif // CTYPECONTROL_H
