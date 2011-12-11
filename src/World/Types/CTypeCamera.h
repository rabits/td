
#ifndef CTYPECAMERA_H
#define CTYPECAMERA_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeCamera
    : public CType
{
public:
    CTypeCamera();

protected:
    CTypeParameter<uint>        m_Range;
    CTypeParameter<std::string> m_Type;
    CTypeParameter<std::string> m_Person;
    CTypeParameter<uint>        m_LookRangeX;
    CTypeParameter<uint>        m_LookRangeY;

private:
};

#endif // CTYPECAMERA_H
