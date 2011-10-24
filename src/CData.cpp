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

CData::CData()
    : m_pDataRoot()
    , m_pData()
{
    m_pData = m_pDataRoot.append_child("td");
    m_pData.append_attribute("version").set_value(CONFIG_TD_VERSION);
}

CData::~CData()
{
    //dtor
}
