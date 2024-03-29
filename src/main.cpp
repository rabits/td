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
 * @param argc
 * @param argv[]
 * @return int
 *
 */
int main(int, char**)
#else
/** @brief Main started function
 *
 * @param hInst - Handle to current instance
 * @param strCmdLine - Pointer to the command file
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
    catch( Common::Exception const& e ) {
        log_emerg("An Common exception has occured: %s", e.getFullDescription().c_str());
    }
    catch( Ogre::Exception const& e ) {
        log_emerg("An Ogre exception has occured: %s", e.getFullDescription().c_str());
    }
    catch( std::exception const& e ) {
        log_emerg("An standart exception has occured: %s", e.what());
    }
    catch(...) {
        log_emerg("An unknown exception has occured!");
    }

    log_notice("Exiting...");

    // Destroy game in the end
    CGame::destroyInstance();

    log_notice("See you...");

    return 0;
}
