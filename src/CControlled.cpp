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


#include "CControlled.h"

CControlled::CControlled()
    : m_Actions()
{
    //ctor
}

CControlled::~CControlled()
{
    //dtor
}

void CControlled::addAction(CAction* pAction)
{
    m_Actions.push_back(pAction);
}

std::vector<CAction*>* CControlled::getActions()
{
    return &m_Actions;
}

void CControlled::clearActions()
{
    std::vector<CAction*>::iterator it = m_Actions.begin();
    for( ; it != m_Actions.end(); it++ )
        delete *it;
    m_Actions.clear();
}
