/**
 * @file    CGame.h
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
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

/*
 * =====================================================================================
 *        Class:  CGame
 *  Description:  Provides all game.
 * =====================================================================================
 */

class CGame : public Ogre::FrameListener, public Ogre::WindowEventListener, OgreBites::SdkTrayListener, CData
{
public:
    ~CGame();

    static CGame* getInstance(void);
    static void destroyInstance(void)
    {
        if( m_pInstance )
            delete m_pInstance;
    };

    bool initialise();
    bool loadConfig();
    void start();
    void getScreenshot();
    void exit();

    // Scene Manager object
    Ogre::SceneManager                      *m_pSceneMgr;
    // Main camera
    Ogre::Camera                            *m_pCamera;
    // Main window
    Ogre::RenderWindow                      *m_pWindow;

    // Input Handler object
    CInputHandler                           *m_pInputHandler;

    // OgreBites
    OgreBites::SdkTrayManager               *m_pTrayMgr;            // Tray Manager object
    OgreBites::ParamsPanel                  *m_pDetailsPanel;        // Sample details panel

    std::vector<CObjectWorld*>::iterator    o_currentWorld;

    // Users list
    std::vector<CUser*>                     m_vUsers;
    std::vector<CUser*>::iterator           o_currentUser;

private:
    // Instance of game
    static CGame                            *m_pInstance;

protected:
    CGame(); // Constructor

    // World list
    std::vector<CObjectWorld*>              m_vWorlds;

    void createFrameListener();
    void updateWorlds(const Ogre::FrameEvent& evt);
    void updateUsers(const Ogre::FrameEvent& evt);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool frameStarted(const Ogre::FrameEvent& evt);

    //Adjust mouse clipping area
    void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root                              *m_pRoot;
    Ogre::Timer                             *m_pTimer;
    // Render next frame in this time (microseconds)
    unsigned long                           m_NextFrameTime;

    bool                                    m_ShutDown;

};

#endif // CGAME_H_INCLUDED
