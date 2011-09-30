/** // CINPUTEVENT_H
 *  ---------------------------
 *   Total Destruction Project
 *   Sergej Parshev       2010
 *  ---------------------------
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
