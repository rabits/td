/**
 * @file    main.cpp
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   Entry point
 *
 *
 */


#include "main.h"

/*
 * =====================================================================================
 *     Function:  main
 * =====================================================================================
 */

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
    printf("Starting program\n");
    try {
        if( CGame::getInstance()->initialise() )
            CGame::getInstance()->start();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
    }
    // Destroy game on end
    CGame::destroyInstance();
    return 0;
}
