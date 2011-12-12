/**
 * @file    CTypeWaveReciever.cpp
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Type wave reciever
 *
 *
 */

#include "World/Types/CTypeWaveReciever.h"

CTypeWaveReciever::CTypeWaveReciever()
    : CType("Wave Reciever", "Recieved waves and filtering its")
    , m_Range("Range", "", 1, 65535)
    , m_Type("Type", "")
    , m_Spectrum("Spectrum", "")
    , m_NoiceReduction("Noice Reduction", "Filtering noices", 0, 65535)
{
    m_Type.addAvailable("Direct", "");
    m_Type.addAvailable("Omni", "");

    m_Spectrum("Sound", "");
    m_Spectrum("Radio", "");
    m_Spectrum("Infrared", "");
    m_Spectrum("Light", "");
    m_Spectrum("UltraViolet", "");
    m_Spectrum("Radiation", "");
}
