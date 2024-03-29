/**
 * @file    CGame.h
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Game master object
 *
 *
 */

#ifndef CGAME_H_INCLUDED
#define CGAME_H_INCLUDED

#include "Common.h"

#include <OIS/OIS.h>

#include <OGRE/SdkTrays.h>

#include <cstdlib>

#include "CData.h"
#include "Nerv/CControlled.h"
#include "World/CWorld.h"

#include "CUser.h"

class CSensor;

/** @brief Provides all game.
 */
class CGame
    : CData
    , Ogre::FrameListener
    , Ogre::WindowEventListener
    , public CControlled
{
public:
    ~CGame();

    /** @brief Create and get current instance of game
     *
     * @return CGame*
     *
     */
    inline static CGame* getInstance() { if( s_pInstance == NULL ) s_pInstance = new CGame(); return s_pInstance; }

    /** @brief Remove main game object
     *
     * @return void
     *
     */
    inline static void   destroyInstance() { delete s_pInstance; }

    /** @brief Get current prefix of game
     *
     * @return const char* - Full path to game prefix
     *
     */
    static const char* getPrefix();


    /** @brief Preparation to game start
     *
     * @return bool
     *
     */
    bool initialise();


    /** @brief Starting game render and playing
     *
     * @return void
     *
     */
    void start();

    /** @brief Shutting down of game
     *
     * @return void
     *
     */
    inline void exit() { m_ShutDown = true; }

    /** @brief Save screenshot
     *
     * @return void
     *
     */
    void getScreenshot();

    /** @brief Get time in milliseconds since game start
     *
     * @return uint - Number of milliseconds since start
     *
     */
    inline uint time() { return m_pTimer->getMilliseconds(); }


    /** @brief Return environment variable
     *
     * @param name
     * @return const char*
     */
    inline const char* env(const char* name) { return m_data.child("env").child_value(name); }

    /** @brief Return path variable
     *
     * @param name
     * @return const char*
     */
    inline const char* path(const char* name) { return m_data.child("path").child_value(name); }

    /** @brief Doing need actions
     *
     * @see CControlled::doAction()
     */
    void doAction(char act, CSignal& sig);

    /** @brief Returns current input handler
     *
     * @return CSensor*
     */
    inline CSensor* inputHandler() { return m_pInputHandler; }


    Ogre::SceneManager*                     m_pSceneMgr; ///< Scene Manager object
    Ogre::Camera*                           m_pCamera; ///< Main camera

    std::vector<CWorld*>::iterator          m_oCurrentWorld; ///< Current processing world

private:
    /** @brief Adding actions
     *
     * @see CControlled::registerActions()
     */
    void registerActions();

    /** @brief Creating one instance of object
     */
    CGame();

    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     */
    CGame(const CGame& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CGame& operator=(const CGame& obj);

    /** @brief Preparing env variables
     *
     * @return bool
     */
    bool loadEnv();

    /** @brief Init OGRE video engine configuration
     *
     * @return bool
     *
     */
    bool initOgre();

    /** @brief Init Bullet physical engine configuration
     *
     * @return bool
     *
     */
    bool initBullet();

    /** @brief Init OIS Nerv controlling system configuration
     *
     * @return bool
     *
     */
    bool initOIS();

    /** @brief Init Sound engine configuration
     *
     * @return bool
     *
     */
    bool initSound();

    /** @brief Init Game misc configuration
     *
     * @return bool
     *
     */
    bool initGame();

    /** @brief This function changes program locale
     *
     * @param messages_path - Path to directory with locale messages was be searched
     * @param locale ("") - Need locale
     */
    void setLocale(const char* messages_path, const char* locale = "");

    /** @brief Frame listener of window
     *
     */
    void createFrameListener();

    /** @brief Main event on update frame
     *
     * @param evt
     * @return bool
     *
     */
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    /** @brief Updating worlds actions on every frame event
     *
     * @param time_since_last_frame
     * @return void
     *
     */
    void updateWorlds(const Ogre::Real time_since_last_frame);

    /** @brief Updating users state on every frame event
     *
     * @param time_since_last_frame
     * @return void
     *
     */
    void updateUsers(const Ogre::Real time_since_last_frame);

    /** @brief Event on start frame
     *
     * @param evt
     * @return bool
     *
     */
    bool frameStarted(const Ogre::FrameEvent& evt);

    /** @brief Event on end frame
     *
     * @param evt
     * @return bool
     *
     */
    bool frameEnded(const Ogre::FrameEvent& evt);


    /** @brief Adjust mouse clipping area
     *
     * @param rw
     * @return void
     *
     */
    void windowResized(Ogre::RenderWindow* rw);

    /** @brief Unattach OIS
     *
     * @param rw
     * @return void
     *
     * Unattach OIS before window shutdown (very important under Linux)
     */
    void windowClosed(Ogre::RenderWindow* rw);

    static CGame*                           s_pInstance; ///< Instance of game
    static fs::path*                        s_pPrefix; ///< Current prefix directory

    CSensor*                                m_pInputHandler; ///< Input Handler object
    Ogre::RenderWindow*                     m_pWindow; ///< Main window

    Ogre::Root*                             m_pRoot; ///< Root Ogre object
    Ogre::LogManager*                       m_pLogManager; ///< Log manager for replacement OGRE default logger
    Ogre::Timer*                            m_pTimer; ///< Game timer for restriction of frame rendering speed
    unsigned long                           m_NextFrameTime; ///< Render next frame in this time (microseconds)

    std::vector<CWorld*>                    m_Worlds; ///< Worlds list

    CUser*                                  m_pMainUser; ///< Link to main user
    std::vector<CUser*>                     m_Users; ///< Users list
    std::vector<CUser*>::iterator           m_oCurrentUser; ///< Current processing user

    bool                                    m_ShutDown; ///< Game need to stop

protected:
};

#endif // CGAME_H_INCLUDED
