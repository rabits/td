/**
 * @file    CInputHandler.h
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   Input handler
 *
 *
 */


#ifndef CINPUTHANDLER_H
#define CINPUTHANDLER_H

#include "config.h"

#include "CGame.h"

#include <OIS/OISForceFeedback.h>

class CInputHandler : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
public:
    CInputHandler(size_t windowHnd);
    ~CInputHandler();

    // Capture events
    void capture();

    OIS::Mouse* getMouse();
    OIS::Keyboard* getKeyboard();
    OIS::JoyStick* getJoyStick(int id);

protected:
    // Keyboard events
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );

    // Mouse events
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Joystick events
    bool povMoved( const OIS::JoyStickEvent &arg, int pov );
    bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
    bool buttonReleased( const OIS::JoyStickEvent &arg, int button );
    bool axisMoved( const OIS::JoyStickEvent &arg, int axis );

    // Input devices
    OIS::InputManager                    *m_pInputManager;

    OIS::Mouse                            *m_pMouse;
    OIS::Keyboard                        *m_pKeyboard;
    OIS::JoyStick                        *m_pJoyStick[CONFIG_JOYSTICK_MAX_NUMBER];
    int                                    m_joysticsNum;            // Number of preset joystics

    CGame                                *m_pGame;
private:
};

#endif // CINPUTHANDLER_H
