
#include "World/Types/CTypeCamera.h"

CTypeCamera::CTypeCamera()
    : CType("Camera", "Displays world around")
    , m_Range("Range", "Maximum view distance", 1, 65535)
    , m_Type("Type", "Display data format")
    , m_Person("Person", "View person")
    , m_LookRangeX("Look Range X", "Maximum horizontal free in degrees", 0, 360)
    , m_LookRangeY("Look Range Y", "Maximum vertical free in degrees", 0, 180)
{
    m_Type.addAvailable("Simple", "Simplificity view");
    m_Type.addAvailable("Normal", "Normal real view");
    m_Type.addAvailable("Night", "Night vision");
    m_Type.addAvailable("InfraRed", "View in IR diapasone - displays temperature of objects");
    m_Type.addAvailable("UltraViolet", "View in UV diapasone - unvisible markers and other");

    m_Person.addAvailable("Third", "1rd view from eyes");
    m_Person.addAvailable("First", "3rd view from back");
}
