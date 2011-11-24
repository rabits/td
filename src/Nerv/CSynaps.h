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
     * @param id unsigned int
     * @param act CAction*
     * @param sens float
     */
    CSynaps(unsigned int id, CAction* act, float sens = 0.0f);

    /** @brief Destructor
     */
    ~CSynaps();

    /** @brief Get Id of catchable signal
     *
     * @return unsigned int
     *
     */
    inline unsigned int id() const { return m_Id; }

    /** @brief Set coefficient of sensitivity
     *
     * @param sens float - coefficient
     */
    inline void sensitivity(float sens) { m_Sensitivity = sens; }

    /** @brief Set coefficient of sensitivity
     *
     * @param sens float - coefficient
     */
    inline float sensitivity() const { return m_Sensitivity; }

    /** @brief Route and change Signal by Synaps settings
     *
     * @param sig CSignal&
     */
    void route(CSignal& sig);

protected:
    unsigned int m_Id;          ///< Signal id
    CAction*     m_Action;      ///< Reference to connected Action
    float        m_Sensitivity; ///< Sensitivity changes

private:
};

#endif // CSynaps_H
