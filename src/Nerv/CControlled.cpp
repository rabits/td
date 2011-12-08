/**
 * @file    CControlled.cpp
 * @date    2010-10-07T15:55:38+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Controlled element
 *
 *
 */


#include "Nerv/CControlled.h"

std::map<uint, CControlled*> CControlled::s_ControlledObjects;
uint CControlled::s_LastId = 0;

CControlled::CControlled(const char* name)
    : CMaster(name)
    , m_Actions()
    , m_Id(CControlled::s_LastId++)
{
    s_ControlledObjects[m_Id] = this;
    log_debug("Registered new controlled object \"%s\" id#%d", this->name().c_str(), m_Id);
}

CControlled::~CControlled()
{
    clearActions();
    s_ControlledObjects[m_Id] = NULL;
    s_ControlledObjects.erase(m_Id);
    log_debug("Removed controlled object \"%s\" id#%d", this->name().c_str(), m_Id);
}

CAction* CControlled::getAction(const char *name)
{
    for( auto it = m_Actions.begin(); it != m_Actions.end(); it++ )
        if( **it == name ) return *it;

    return NULL;
}

void CControlled::clearActions()
{
    auto it = m_Actions.begin();
    for( ; it != m_Actions.end(); it++ )
        delete *it;
    m_Actions.clear();
}
