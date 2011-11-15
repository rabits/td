/**
 * @file    CSignal.h
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

#ifndef CSIGNAL_H
#define CSIGNAL_H

#include "Common.h"

#include <OIS/OIS.h>

/** @brief Unified event from controller (keyboard, mouse)
 */
class CSignal
{
public:
    /** @brief Constructor for float
     */
    CSignal(unsigned int id, float value);

    /** @brief Destructor
     */
    ~CSignal();

    /** Signal type
     */
    enum Type
    {
        SIGNAL_ABSOLUTE, ///< from min to max (like button)
        SIGNAL_RELATIVE  ///< relative state (like mouse)
    };

    /** @brief Get Id of event
     *
     * @return unsigned int
     *
     */
    unsigned int id();

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
     * @return Type - from CSignal::Type
     */
    Type type();

    /** @brief Set type of value
     *
     * @param type Type - new type
     */
    void type(Type type);

protected:
    unsigned int m_Id;    ///< Id
    float        m_Value; ///< Value
    Type         m_Type;  ///< Type of value

private:
};

#endif // CSIGNAL_H
