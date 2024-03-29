/**
 * @file    CSensor.h
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

#ifndef CSENSOR_H
#define CSENSOR_H

#include "Common.h"

#include <OIS/OISForceFeedback.h>

#include "CGame.h"
#include "Nerv/CSignal.h"

typedef std::map<uint, CUser*> SigUser; ///< Signal->User map
typedef std::map<OIS::Type, SigUser > SubUsers; ///< Device->SigUser map - maybe no-need...

/** @brief Global input handler from user and routed it in need user
 */
class CSensor : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
public:
    /** @brief Constructor of global input handler
     *
     * @param windowHnd
     *
     */
    CSensor(size_t windowHnd);

    /** @brief Destructor
     */
    ~CSensor();

    enum {
        SENS_LEFT      = 0, ///< X Left
        SENS_RIGHT     = 1, ///< X Right
        SENS_UP        = 2, ///< Y Up
        SENS_DOWN      = 3, ///< Y Down
        SENS_IMMERSION = 4, ///< Z Up
        SENS_EMERSION  = 5  ///< Z Down
    };

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
     * @param joyId
     * @return OIS::JoyStick*
     *
     */
    OIS::JoyStick* getJoyStick(int joyId);

    /** @brief Add subscribe on event with special id
     *
     * @param id
     * @param pUser
     */
    bool addSubscribe(uint id, CUser* pUser);

    /** @brief Delete subscribe on event
     *
     * @param id
     */
    bool delSubscribe(uint id);

protected:
    /** @brief Function on keyboard key pressed event
     *
     * @param arg
     * @return bool
     *
     */
    bool keyPressed( const OIS::KeyEvent& arg );

    /** @brief Function on keyboard key released event
     *
     * @param arg
     * @return bool
     *
     */
    bool keyReleased( const OIS::KeyEvent& arg );


    /** @brief Function on mouse moved event
     *
     * @param arg
     * @return bool
     *
     */
    bool mouseMoved( const OIS::MouseEvent& arg );

    /** @brief Function on mouse button pressed
     *
     * @param arg
     * @param id
     * @return bool
     *
     */
    bool mousePressed( const OIS::MouseEvent& arg, OIS::MouseButtonID id );

    /** @brief Function on mouse button released
     *
     * @param arg
     * @param id
     * @return bool
     *
     */
    bool mouseReleased( const OIS::MouseEvent& arg, OIS::MouseButtonID id );


    /** @brief Function on joystick cross pov pressed
     *
     * @param arg
     * @param pov
     * @return bool
     *
     */
    bool povMoved( const OIS::JoyStickEvent& arg, int pov );

    /** @brief Function on joystick button pressed
     *
     * @param arg
     * @param button
     * @return bool
     *
     */
    bool buttonPressed( const OIS::JoyStickEvent& arg, int button );

    /** @brief Function on joystick button released
     *
     * @param arg
     * @param button
     * @return bool
     *
     */
    bool buttonReleased( const OIS::JoyStickEvent& arg, int button );

    /** @brief Function on joystick stick moved
     *
     * @param arg
     * @param axis
     * @return bool
     *
     */
    bool axisMoved( const OIS::JoyStickEvent& arg, int axis );

    OIS::InputManager* m_pInputManager; ///< OIS input manager
    OIS::Mouse*        m_pMouse; ///< Mouse device
    OIS::Keyboard*     m_pKeyboard; ///< Keyboard device
    OIS::JoyStick*     m_pJoyStick[CONFIG_JOYSTICK_MAX_NUMBER]; ///< Joysticks devices

    int                m_JoysticsNum; ///< Number of preset joystics

    CGame*             m_pGame; ///< Link to game

    SubUsers           m_subscribedUsers; ///< Map of subscribed Users to need signals. DeviceType->EventType->UserLink

private:
    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     */
    CSensor(const CSensor& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CSensor& operator=(const CSensor& obj);

    inline uint genId(OIS::Type dev, int dev_number, int button){ return static_cast<uint>(dev) * 10000u + static_cast<uint>(dev_number) * 1000u + static_cast<uint>(button); }

    std::string        m_DeviceType[6]; ///< Device types

    int m_CleanMouse; ///< Frames to clean mouse device move
    int m_LastMouseX; ///< Last mouse move X
    int m_LastMouseY; ///< Last mouse move Y
    int m_LastMouseZ; ///< Last mouse move Z

    int m_LastPovX;     ///< Last joystick pov X
    int m_LastPovY;     ///< Last joystick pov Y

    int m_JoyStickZero; ///< Zero for sticks on joystick
    bool m_ChangedAxis[16]; ///< Axis of joystick has changed and need to send zero Signal
};

#endif // CSENSOR_H
