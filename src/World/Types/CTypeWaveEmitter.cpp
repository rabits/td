/**
 * @file    CTypeWaveEmitter.cpp
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

#include "World/Types/CTypeWaveEmitter.h"

CTypeWaveEmitter::CTypeWaveEmitter()
    : CType("Wave Emitter", "Emitting of any waves")
    , m_Power("Power", "", 1, 65535)
    , m_Type("Type", "")
    , m_Directivity("Directivity", "")
    , m_Spectrum("Spectrum", "")
{
    m_Type.addAvailable("Noice", "");
    m_Type.addAvailable("Info", "");

    m_Directivity("Direct", "");
    m_Directivity("Omni", "");

    m_Spectrum("Sound", "");
    m_Spectrum("Radio", "");
    m_Spectrum("Infrared", "");
    m_Spectrum("Light", "");
    m_Spectrum("UltraViolet", "");
    m_Spectrum("Radiation", "");
}
