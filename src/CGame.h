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

#include "config.h"

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include <OGRE/SdkTrays.h>

#include <cstdlib>
#include <vector>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem3;

#include "CData.h"
#include "CObject.h"
#include "CObjectCube.h"
#include "CObjectWorld.h"
#include "CObjectKernel.h"

#include "CUser.h"

class CInputHandler;

/** @brief Provides all game.
 */
class CGame : public Ogre::FrameListener, public Ogre::WindowEventListener, OgreBites::SdkTrayListener, CData
{
public:
    ~CGame();

    static CGame* getInstance();
    static void   destroyInstance();

    bool initialise();
    void start();
    void getScreenshot();
    void exit();

    Ogre::SceneManager                      *m_pSceneMgr; ///< Scene Manager object
    Ogre::Camera                            *m_pCamera; ///< Main camera
    Ogre::RenderWindow                      *m_pWindow; ///< Main window

    CInputHandler                           *m_pInputHandler; ///< Input Handler object

    OgreBites::SdkTrayManager               *m_pTrayMgr; ///< Tray Manager object
    OgreBites::ParamsPanel                  *m_pDetailsPanel; ///< Sample details panel

    std::vector<CUser*>                     m_vUsers; ///< Users list
    std::vector<CUser*>::iterator           o_currentUser; ///< Current processing user

    std::vector<CObjectWorld*>::iterator    o_currentWorld; ///< Current processing world

private:
    static CGame                            *m_pInstance; ///< Instance of game

protected:
    CGame();

    std::vector<CObjectWorld*>              m_vWorlds; ///< Worlds list

    bool loadConfig();

    void createFrameListener();
    void updateWorlds(const Ogre::FrameEvent& evt);
    void updateUsers(const Ogre::FrameEvent& evt);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool frameStarted(const Ogre::FrameEvent& evt);

    void windowResized(Ogre::RenderWindow* rw);
    void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root                              *m_pRoot; ///< Root Ogre object
    Ogre::Timer                             *m_pTimer; ///< Game timer for restriction of frame rendering speed
    unsigned long                           m_NextFrameTime; ///< Render next frame in this time (microseconds)

    bool                                    m_ShutDown; ///< Game need to stop

};

#endif // CGAME_H_INCLUDED
