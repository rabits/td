/**
 * @file    CInputEvent.h
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   Input event
 *
 *
 */


#ifndef CINPUTEVENT_H
#define CINPUTEVENT_H

#include "config.h"

#include <OIS/OIS.h>

class CInputEvent
{
public:
    CInputEvent();
    ~CInputEvent();
protected:
    // KeyBoard, Mouse or JoyStick
    OIS::Type m_inputType;

    // Use static force
    bool m_staticUse;
    int m_staticValue;
private:
};

#endif // CINPUTEVENT_H
