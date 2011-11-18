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

std::map<unsigned int, CControlled*> CControlled::s_ControlledObjects;
unsigned int CControlled::s_LastId = 0;

CControlled::CControlled()
    : m_Actions()
    , m_Id(CControlled::s_LastId++)
{
    s_ControlledObjects[m_Id] = this;
}

CControlled::~CControlled()
{
    clearActions();
    s_ControlledObjects[m_Id] = NULL;
    s_ControlledObjects.erase(m_Id);
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
