/**
 * @file    CInputEvent.h
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Input event
 *
 *
 */

#ifndef CINPUTEVENT_H
#define CINPUTEVENT_H

#include "Common.h"

#include <OIS/OIS.h>

/** @brief Unified event from controller (keyboard, mouse)
 */
class CInputEvent
{
public:
    /** @brief Constructor for float
     */
    CInputEvent(int id, float value);

    /** @brief Destructor
     */
    ~CInputEvent();

    enum Type
    {
        EVENT_ABSOLUTE, ///< from min to max (like button)
        EVENT_RELATIVE  ///< relative state (like mouse)
    };

    /** @brief Get Id of event
     *
     * @return int
     *
     */
    int id();

    /** @brief Get Value of event
     *
     * @return float
     *
     */
    float value();

    /** @brief Set coefficient of sensitivity
     *
     * @param sens float - coefficient
     */
    void setSens(float sens);

    /** @brief Get type of value
     *
     * @return Type - from CInputEvent::Type
     */
    Type type();

    /** @brief Set type of value
     *
     * @param type Type - new type
     */
    void type(Type type);

protected:
    float  m_Id;    ///< Id
    float  m_Value; ///< Value
    Type   m_Type;  ///< Type of value

private:
};

#endif // CINPUTEVENT_H
