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

#include "config.h"

#include <OIS/OIS.h>

/** @brief Event of user from controller (keyboard, mouse)
 *
 * Have 1 parameter:
 * - value (if button: 0.0,1.0; if stick: 0.0...1.0)
 */
class CInputEvent
{
public:
    /** @brief Constructor
     */
    CInputEvent();

    /** @brief Destructor
     */
    ~CInputEvent();
protected:
    float  m_value; ///< Value of event
private:
};

#endif // CINPUTEVENT_H
