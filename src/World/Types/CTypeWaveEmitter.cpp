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

    m_Directivity.addAvailable("Direct", "");
    m_Directivity.addAvailable("Omni", "");

    m_Spectrum.addAvailable("Sound", "");
    m_Spectrum.addAvailable("Radio", "");
    m_Spectrum.addAvailable("Infrared", "");
    m_Spectrum.addAvailable("Light", "");
    m_Spectrum.addAvailable("UltraViolet", "");
    m_Spectrum.addAvailable("Radiation", "");
}
