/**
 * @file    CControlled.h
 * @date    2010-10-07T15:55:38+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Controlled object
 *
 *
 */

#ifndef CCONTROLLED_H
#define CCONTROLLED_H

#include <vector>
#include <string>

#include "CAction.h"

/** @brief Object with this class can be controlled by actions
 */
class CControlled
{
public:
    CControlled();
    virtual ~CControlled();

    std::vector<CAction>        getAvailableActions();
protected:
    std::vector<CAction>        m_vActions; ///< Actions of object to be controlled

private:
};

#endif // CCONTROLLED_H
