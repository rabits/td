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

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include <OGRE/SdkTrays.h>

#include <cstdlib>

#include "CData.h"
#include "CWorld.h"

#include "CUser.h"

class CInputHandler;

/** @brief Provides all game.
 */
class CGame
    : CData
    , Ogre::FrameListener
    , Ogre::WindowEventListener
    , OgreBites::SdkTrayListener
{
public:
    ~CGame();

    /** @brief Create and get current instance of game
     *
     * @param bindir const wchar_t* - Full path to binary directory (default is NULL)
     * @return CGame*
     *
     */
    static CGame* getInstance(){ if( m_pInstance == NULL ) m_pInstance = new CGame(); return m_pInstance; }

    /** @brief Remove main game object
     *
     * @return void
     *
     */
    static void   destroyInstance(){ delete m_pInstance; }

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
     * Loading env
     * Loading global config
     * Loading user config
     * Create root object of Ogre
     * Load config of resources
     * Initialise root object of Ogre
     * Create render window
     * Choising screen manager
     * Creating main camera
     * Creating viewport
     * Load resources
     * Create frame listener
     * Create worlds
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
    void exit(){ m_ShutDown = true; }

    /** @brief Save screenshot
     *
     * @return void
     *
     */
    void getScreenshot();

    /** @brief Get time in milliseconds since game start
     *
     * @return unsigned int - Number of milliseconds since start
     *
     */
    unsigned int time(){ return m_pTimer->getMilliseconds(); }


    /** @brief Return environment variable
     *
     * @param name char const*
     * @return char const*
     */
    const char* env(const char* name){ return m_data.child("env").child_value(name); }

    /** @brief Return path variable
     *
     * @param name const char*
     * @return const char*
     */
    const char* path(const char* name){ return m_data.child("path").child_value(name); }


    Ogre::SceneManager*                     m_pSceneMgr; ///< Scene Manager object
    Ogre::Camera*                           m_pCamera; ///< Main camera
    Ogre::RenderWindow*                     m_pWindow; ///< Main window

    CInputHandler*                          m_pInputHandler; ///< Input Handler object

    OgreBites::SdkTrayManager*              m_pTrayMgr; ///< Tray Manager object
    OgreBites::ParamsPanel*                 m_pDetailsPanel; ///< Sample details panel

    CUser*                                  m_pMainUser; ///< Link to main user
    std::vector<CUser*>                     m_Users; ///< Users list
    std::vector<CUser*>::iterator           m_oCurrentUser; ///< Current processing user

    std::vector<CWorld*>::iterator          m_oCurrentWorld; ///< Current processing world

private:
    static CGame*                           m_pInstance; ///< Instance of game
    static fs::path*                        m_pPrefix; ///< Current prefix directory

protected:
    /** @brief Creating one instance of object
     */
    CGame();

    /** @brief Fake copy constructor
     */
    CGame(const CGame &obj);

    std::vector<CWorld*>                    m_Worlds; ///< Worlds list

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

    /** @brief Frame listener of info panel
     *
     * @return void
     *
     */
    void createFrameListener();

    /** @brief Main event on update frame
     *
     * @param evt const Ogre::FrameEvent&
     * @return bool
     *
     */
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    /** @brief Updating worlds actions on every frame event
     *
     * @param evt const Ogre::FrameEvent&
     * @return void
     *
     */
    void updateWorlds(const Ogre::FrameEvent& evt);

    /** @brief Updating users state on every frame event
     *
     * @param evt const Ogre::FrameEvent&
     * @return void
     *
     */
    void updateUsers(const Ogre::FrameEvent& evt);

    /** @brief Event on start frame
     *
     * @param evt const Ogre::FrameEvent&
     * @return bool
     *
     */
    bool frameStarted(const Ogre::FrameEvent& evt);


    /** @brief Adjust mouse clipping area
     *
     * @param rw Ogre::RenderWindow*
     * @return void
     *
     */
    void windowResized(Ogre::RenderWindow* rw);

    /** @brief Unattach OIS
     *
     * @param rw Ogre::RenderWindow*
     * @return void
     *
     * Unattach OIS before window shutdown (very important under Linux)
     */
    void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root*                             m_pRoot; ///< Root Ogre object
    Ogre::LogManager*                       m_pLogManager; ///< Log manager for replacement OGRE default logger
    Ogre::Timer*                            m_pTimer; ///< Game timer for restriction of frame rendering speed
    unsigned long                           m_NextFrameTime; ///< Render next frame in this time (microseconds)

    bool                                    m_ShutDown; ///< Game need to stop

};

#endif // CGAME_H_INCLUDED
