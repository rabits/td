/**
 * @file    CInputHandler.h
 * @date    2010-09-29T22:42:56+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Input handler
 *
 *
 */

#ifndef CINPUTHANDLER_H
#define CINPUTHANDLER_H

#include "Common.h"

#include <OIS/OISForceFeedback.h>

#include "CGame.h"
#include "CInputEvent.h"

/** @brief Global input handler from user and routed it in need user
 */
class CInputHandler : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
public:
    /** @brief Constructor of global input handler
     *
     * @param windowHnd size_t
     *
     */
    CInputHandler(size_t windowHnd);

    /** @brief Copy constructor of global input handler
     *
     * @param obj const CInputHandler&
     *
     */
    CInputHandler(const CInputHandler &obj);

    /** @brief Destructor
     */
    ~CInputHandler();


    /** @brief Capture of events in mouse, keyboard or joystics
     *
     * @return void
     *
     */
    void capture();


    /** @brief Get connected mouse device
     *
     * @return OIS::Mouse*
     *
     */
    OIS::Mouse*    getMouse();

    /** @brief Get connected keyboard device
     *
     * @return OIS::Keyboard*
     *
     */
    OIS::Keyboard* getKeyboard();

    /** @brief Get joystick by id
     *
     * @param joyId int
     * @return OIS::JoyStick*
     *
     */
    OIS::JoyStick* getJoyStick(int joyId);

protected:
    /** @brief Function on keyboard key pressed event
     *
     * @param arg const OIS::KeyEvent&
     * @return bool
     *
     */
    bool keyPressed( const OIS::KeyEvent &arg );

    /** @brief Function on keyboard key released event
     *
     * @param arg const OIS::KeyEvent&
     * @return bool
     *
     */
    bool keyReleased( const OIS::KeyEvent &arg );


    /** @brief Function on mouse moved event
     *
     * @param arg const OIS::MouseEvent&
     * @return bool
     *
     */
    bool mouseMoved( const OIS::MouseEvent &arg );

    /** @brief Function on mouse button pressed
     *
     * @param arg const OIS::MouseEvent&
     * @param id OIS::MouseButtonID
     * @return bool
     *
     */
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    /** @brief Function on mouse button released
     *
     * @param arg const OIS::MouseEvent&
     * @param id OIS::MouseButtonID
     * @return bool
     *
     */
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


    /** @brief Function on joystick cross pov pressed
     *
     * @param arg const OIS::JoyStickEvent&
     * @param pov int
     * @return bool
     *
     */
    bool povMoved( const OIS::JoyStickEvent &arg, int pov );

    /** @brief Function on joystick button pressed
     *
     * @param arg const OIS::JoyStickEvent&
     * @param button int
     * @return bool
     *
     */
    bool buttonPressed( const OIS::JoyStickEvent &arg, int button );

    /** @brief Function on joystick button released
     *
     * @param arg const OIS::JoyStickEvent&
     * @param button int
     * @return bool
     *
     */
    bool buttonReleased( const OIS::JoyStickEvent &arg, int button );

    /** @brief Function on joystick stick moved
     *
     * @param arg const OIS::JoyStickEvent&
     * @param axis int
     * @return bool
     *
     */
    bool axisMoved( const OIS::JoyStickEvent &arg, int axis );

    OIS::InputManager *m_pInputManager; ///< OIS input manager
    OIS::Mouse        *m_pMouse; ///< Mouse device
    OIS::Keyboard     *m_pKeyboard; ///< Keyboard device
    OIS::JoyStick     *m_pJoyStick[CONFIG_JOYSTICK_MAX_NUMBER]; ///< Joysticks devices

    int                m_joysticsNum; ///< Number of preset joystics

    CGame             *m_pGame; ///< Link to game
private:
};

#endif // CINPUTHANDLER_H
