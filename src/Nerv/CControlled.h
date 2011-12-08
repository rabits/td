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

#include "CMaster.h"
#include "Nerv/CAction.h"
#include "Nerv/CSignal.h"

/** @brief Object with this class can be controlled by actions
 */
class CControlled
    : CMaster
{
public:
    /** @brief Constructor with name
     *
     * @param name const char*
     */
    CControlled(const char* name);
    virtual ~CControlled();

    /** @brief Controlled object from global list by id
     *
     * @param id
     * @return CControlled*
     */
    static CControlled* getControlledObject(uint id){ return s_ControlledObjects.find(id)->second; }

    /** @brief Return list of actions
     *
     * @return std::vector<CAction*>*
     *
     */
    std::vector<CAction*>* getActions(){ return &m_Actions; }

    /** @brief Get action by name
     *
     * @param name const char* - name of searched action
     * @return CAction*
     */
    CAction* getAction(const char* name);

    /** @brief Doing hardcoded actions
     *
     * @param act char - ident of action
     * @param sig CSignal& - signal from Sensor
     *
     * C++ not provided refs to object member functions.
     * This is bad realization of my idea - maybe need to use delegates...
     *
     * @todo Try to realize delegates: http://www.rsdn.ru/article/cpp/fastdelegate.xml or boost::function
     */
    virtual void doAction(char act, CSignal& sig) = 0;

protected:
    /** @brief Add action
     *
     * @param act char - Identifier in object
     * @param name const char* - Readable name
     * @return void
     *
     */
    void addAction(char act, const char* name){ m_Actions.push_back(new CAction(this, act, name)); }

    /** @brief Delete all actions
     *
     * @return void
     *
     */
    void clearActions();

    /** @brief Register object actions
     *
     * Will be overrided by any controlled object for register self actions
     */
    virtual void registerActions() = 0;

    std::vector<CAction*>                m_Actions; ///< List of this object actions
    uint                         m_Id;      ///< Id of controlled object

private:
    static std::map<uint, CControlled*> s_ControlledObjects; ///< List of all created controlled objects
    static uint                         s_LastId;            ///< Greatest id
};

#endif // CCONTROLLED_H
