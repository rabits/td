/**
 * @file    CAction.h
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Action of Controlled object
 *
 *
 */

#ifndef CACTION_H
#define CACTION_H

#include "Common.h"
#include "Nerv/CSignal.h"

class CControlled;

/** @brief Action, registered by Controlled object for controlling self
 */
class CAction
{
public:
    /** @brief Constructor for action
     *
     * @param obj - Controlled object of action
     * @param act - Identificator of action in controlled object
     * @param name - Readable name of action
     */
    CAction(CControlled* obj, char act, const char* name);
    ~CAction();

    /** @brief Do attached action
     *
     * @param sig
     */
    void action(CSignal& sig) const;

    /** @brief Get name of action
     *
     * @return const char*
     */
    const char* name() const { return m_Name.c_str(); }

    /** @brief Comparing by name with char string
     *
     * @param name
     * @return bool
     */
    bool operator==(const char* name){ return m_Name.compare(name) == 0; }

protected:
private:
    /** @brief copy constructor
     *
     * @param obj
     */
    CAction(const CAction& obj);
    /** @brief Copy operator
     *
     * @param obj
     * @return CAction& operator
     */
    CAction& operator=(const CAction& obj);

    std::string        m_Name;     ///< Name of action
    CControlled*       m_pObject;  ///< Controlled object

    char               m_Action;   ///< Action in object
};

#endif // CACTION_H
