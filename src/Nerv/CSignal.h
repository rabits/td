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
    /** @brief Default constructor
     */
    CSignal();

    /** @brief Constructor
     *
     * @param id
     * @param value
     * @param sens
     * @param limit
     */
    CSignal(uint id, float value, float sens = 1.0f, float limit = 0.0f);

    /** @brief Destructor
     */
    ~CSignal();

    /** @brief Get Id of signal
     *
     * @return uint
     *
     */
    inline uint id() { return m_Id; }

    /** @brief Get Value of event
     *
     * @return float
     *
     */
    inline float value() { return (m_Limit > m_Value) ? 0.0f : std::min(m_Value * m_Sensitivity, 1.0f); }

    /** @brief Set coefficient of sensitivity
     *
     * @param sens - coefficient of value
     */
    inline void sensitivity(float sens) { m_Sensitivity = sens; }

    /** @brief Get coefficient
     *
     * @return float - coefficient of value
     */
    inline float sensitivity() const { return m_Sensitivity; }

    /** @brief Set minimal non-zero value
     *
     * @param limit - Minimal non-zero value
     */
    inline void limit(float limit) { m_Limit = limit; }

    /** @brief Get minimal non-zero value
     *
     * @return float - Minimal non-zero value
     */
    inline float limit() const { return m_Limit; }

protected:
    uint m_Id;    ///< Id
    float        m_Value; ///< Value
    float        m_Sensitivity; ///< Sensitivity
    float        m_Limit;       ///< Minimal non-zero value

private:
};

#endif // CSIGNAL_H
