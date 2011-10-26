/**
 * @file    CData.cpp
 * @date    2011-10-22T17:01:20+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   XML data container for object
 *
 *
 */

#include "CData.h"

CData::CData(const char *name)
    : m_dataRoot()
    , m_data()
{
    m_data = m_dataRoot.append_child(CONFIG_TD_NAME).append_child(name);
    m_dataRoot.child(CONFIG_TD_NAME).append_attribute("version").set_value(CONFIG_TD_VERSION);
}

CData::~CData()
{
}

void CData::saveData(std::ostream &stream)
{
    m_dataRoot.save(stream, "  ", pugi::format_default, pugi::xml_encoding::encoding_utf8);
}
