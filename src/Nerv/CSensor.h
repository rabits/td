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

typedef std::map<unsigned int, CUser*> SigUser; ///< Signal->User map
typedef std::map<OIS::Type, SigUser > SubUsers; ///< Device->SigUser map - maybe no-need...

/** @brief Global input handler from user and routed it in need user
 */
class CSensor : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
public:
    /** @brief Constructor of global input handler
     *
     * @param windowHnd size_t
     *
     */
    CSensor(size_t windowHnd);

    /** @brief Destructor
     */
    ~CSensor();


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

    /** @brief Add subscribe on event with special id
     *
     * @param id unsigned int
     * @param pUser CUser*
     */
    bool addSubscribe(unsigned int id, CUser* pUser);

    /** @brief Delete subscribe on event
     *
     * @param id unsigned int
     */
    bool delSubscribe(unsigned int id);

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

    OIS::InputManager* m_pInputManager; ///< OIS input manager
    OIS::Mouse*        m_pMouse; ///< Mouse device
    OIS::Keyboard*     m_pKeyboard; ///< Keyboard device
    OIS::JoyStick*     m_pJoyStick[CONFIG_JOYSTICK_MAX_NUMBER]; ///< Joysticks devices

    int                m_joysticsNum; ///< Number of preset joystics

    CGame*             m_pGame; ///< Link to game

    SubUsers           m_subscribedUsers; ///< Map of subscribed Users to need signals. DeviceType->EventType->UserLink

private:
    /** @brief Fake copy constructor
     *
     * @param obj
     */
    CSensor(const CSensor& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     */
    CSensor& operator=(const CSensor& obj);
};

#endif // CSENSOR_H
