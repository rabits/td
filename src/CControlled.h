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

#include "Common.h"

#include "CAction.h"

/** @brief Object with this class can be controlled by actions
 */
class CControlled
{
public:
    CControlled();
    virtual ~CControlled();

    /** @brief Add action
     *
     * @param pAction CAction*
     * @return void
     *
     */
    void addAction(CAction* pAction);

    /** @brief Return list of actions
     *
     * @return std::vector<CAction*>*
     *
     */
    std::vector<CAction*>* getActions();

    /** @brief Delete all actions
     *
     * @return void
     *
     */
    void clearActions();

protected:
    std::vector<CAction*>                m_Actions; ///< List of this object actions

private:
};

#endif // CCONTROLLED_H
