/**
 * @file    CSynaps.h
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
#ifndef CSYNAPS_H
#define CSYNAPS_H

#include "Common.h"

class CAction;
class CSignal;

/** @brief Mapper to connect Signal and Action with need changes.
 */
class CSynaps
{
public:
    /** @brief Constructor
     *
     * @param id uint
     * @param act CAction*
     * @param sens float
     * @param limit float
     */
    CSynaps(uint id, CAction* act, float sens = 0.0f, float limit = 0.0f);

    /** @brief Destructor
     */
    ~CSynaps();

    /** @brief Get Id of catchable signal
     *
     * @return uint
     *
     */
    inline uint id() const { return m_Id; }

    /** @brief Set coefficient of sensitivity
     *
     * @param sens float - coefficient of value
     */
    inline void sensitivity(float sens) { if( sens > 0.0f ) m_Sensitivity = sens; }

    /** @brief Get coefficient
     *
     * @return float - coefficient of value
     */
    inline float sensitivity() const { return m_Sensitivity; }

    /** @brief Set minimal non-zero value
     *
     * @param limit float - Minimal non-zero value
     */
    inline void limit(float limit) { if( limit > 0.0f ) m_Limit = limit; }

    /** @brief Get minimal non-zero value
     *
     * @return float - Minimal non-zero value
     */
    inline float limit() const { return m_Limit; }

    /** @brief Route and change Signal by Synaps settings
     *
     * @param sig CSignal&
     */
    void route(CSignal& sig);

protected:
    uint m_Id;          ///< Signal id
    CAction*     m_Action;      ///< Reference to connected Action
    float        m_Sensitivity; ///< Sensitivity changes
    float        m_Limit;       ///< Minimal non-zero value
    float        m_LastValue;   ///< Last sended signal

private:
    /** @brief Fake copy constructor
     *
     * @param obj const CSynaps&
     *
     * @todo create copy constructor
     */
    CSynaps(const CSynaps& obj);
    /** @brief Fake eq operator
     *
     * @param obj const CSynaps&
     *
     * @toto create eq copy operator
     */
    CSynaps& operator=(const CSynaps& obj);
};

#endif // CSynaps_H
