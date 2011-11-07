/**
 * @file    main.cpp
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Entry point
 *
 *
 */

#include "main.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32

// Catching signals by exceptions
Common::SignalTranslator<Common::SegmentationFault> g_objSegmentationFaultTranslator;
Common::SignalTranslator<Common::FloatingPointException> g_objFloatingPointExceptionTranslator;

/** @brief Main started function
 *
 * @param argc int
 * @param argv[] char*
 * @return int
 *
 */
int main(int argc, char* argv[])
#else
/** @brief Main started function
 *
 * @param hInst HINSTANCE - Handle to current instance
 * @param strCmdLine LPSTR - Pointer to the command file
 * @return INT WINAPI
 *
 */
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#endif
{
    log_notice("Starting %s v%s", CONFIG_TD_FULLNAME, CONFIG_TD_VERSION);

    try {
        if( CGame::getInstance()->initialise() )
            CGame::getInstance()->start();
    }
    catch( Ogre::Exception& e ) {
        log_emerg("An exception has occured: %s", e.getFullDescription().c_str());
    }
    catch( std::exception& e ) {
        log_emerg("An standart exception has occured: %s", e.what());
    }
    catch(...) {
        log_emerg("An unknown exception has occured!");
    }

    log_notice("See you...");

    // Destroy game in the end
    CGame::destroyInstance();

    return 0;
}
