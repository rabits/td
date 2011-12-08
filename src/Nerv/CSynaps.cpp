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

CSynaps::CSynaps(uint id, CAction* act, float sens, float limit)
    : m_Id(id)
    , m_Action(act)
    , m_Sensitivity()
    , m_Limit()
    , m_LastValue(0.0f)
{
    if( sens > 0.0f )
        m_Sensitivity = sens;
    if( limit > 0.0f )
        m_Limit = limit;
}

CSynaps::~CSynaps()
{
}

void CSynaps::route(CSignal &sig)
{
    if( m_Sensitivity > 0.0f )
        sig.sensitivity(m_Sensitivity);
    if( m_Limit > 0.0f )
        sig.limit(m_Limit);

    float value = sig.value();

    if( m_Action )
    {
        if( m_LastValue != value )
        {
            m_Action->action(sig);
            m_LastValue = value;
        }
    }
    else
        EXCEPTION("Synaps: Can't provide signal to bad action");
}
