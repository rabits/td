/**
 * @file    CTypeCamera.h
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type camera
 *
 *
 */

#ifndef CTYPECAMERA_H
#define CTYPECAMERA_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeCamera
    : public CType
{
public:
    CTypeCamera();

    void info() const;

protected:
    CTypeParameter<uint>        m_Range;
    CTypeParameter<std::string> m_Type;
    CTypeParameter<std::string> m_Person;
    CTypeParameter<uint>        m_LookRangeX;
    CTypeParameter<uint>        m_LookRangeY;

private:
};

#endif // CTYPECAMERA_H
