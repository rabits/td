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
    /** @brief Constructor
     *
     * @param id unsigned int
     * @param value float
     * @param sens float
     */
    CSignal(unsigned int id, float value, float sens = 1.0f);

    /** @brief Destructor
     */
    ~CSignal();

    /** @brief Get Id of signal
     *
     * @return unsigned int
     *
     */
    inline unsigned int id() { return m_Id; }

    /** @brief Get Value of event
     *
     * @return float
     *
     */
    inline float value() { return std::min(m_Value * m_Sensitivity, 1.0f); }

    /** @brief Set coefficient of sensitivity
     *
     * @param sens float - coefficient
     */
    inline void sensitivity(float sens) { m_Sensitivity = sens; }

    /** @brief Set coefficient of sensitivity
     *
     * @param sens float - coefficient
     */
    inline float sensitivity() { return m_Sensitivity; }

protected:
    unsigned int m_Id;    ///< Id
    float        m_Value; ///< Value
    float        m_Sensitivity; ///< Sensitivity

private:
};

#endif // CSIGNAL_H
