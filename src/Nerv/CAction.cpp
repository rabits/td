/**
 * @file    CAction.cpp
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Action of object
 *
 *
 */

#include "Nerv/CAction.h"
#include "Nerv/CControlled.h"

CAction::CAction(CControlled* obj, char act, const char* name)
    : m_Name(name)
    , m_pObject(obj)
    , m_Action(act)
{
}

CAction::~CAction()
{
}

void CAction::action(CSignal& sig) const
{
    log_debug("ACTION %s: doing signal id#%d value: %f", m_Name.c_str(), sig.id(), sig.value());
    m_pObject->doAction(m_Action, sig);
}
