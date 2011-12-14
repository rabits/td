/**
 * @file    CTypeEnergy.h
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type energy
 *
 *
 */

#ifndef CTYPEENERGY_H
#define CTYPEENERGY_H

#include "Common.h"

#include "World/Types/CType.h"

class CTypeEnergy
    : public CType
{
public:
    CTypeEnergy();

    void info() const;

protected:
    CTypeParameter<float>        m_Energy;
    CTypeParameter<std::string>  m_Type;
    CTypeParameter<float>        m_Regenerate;

private:
};

#endif // CTYPEENERGY_H
