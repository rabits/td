/**
 * @file    CSynaps.cpp
 * @date    2011-11-24T21:08:19+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Synaps recieve Signal from User, changes it and submit to action
 *
 *
 */
#include "Nerv/CSynaps.h"

#include "Nerv/CSignal.h"
#include "Nerv/CAction.h"

CSynaps::CSynaps(unsigned int id, CAction* act, float sens)
    : m_Id(id)
    , m_Action(act)
    , m_Sensitivity(sens)
{
}

CSynaps::~CSynaps()
{
}

void CSynaps::route(CSignal &sig)
{
    if( m_Sensitivity > 0.0f )
        sig.sensitivity(m_Sensitivity);

    if( m_Action )
        m_Action->action(sig);
    else
        EXCEPTION("Synaps: Can't provide signal to bad action");
}
