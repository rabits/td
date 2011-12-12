/**
 * @file    CTypeWaveEmitter.h
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type wave emitter
 *
 *
 */

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
