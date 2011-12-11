
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
