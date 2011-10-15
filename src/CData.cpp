#include "CData.h"

CData::CData()
{
    m_pData = new pugi::xml_document();
    m_pData->child("td").append_attribute("version").set_value(CONFIG_TD_VERSION);
}

CData::~CData()
{
    //dtor
}
