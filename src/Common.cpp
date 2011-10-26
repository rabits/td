/**
 * @file    Common.cpp
 * @date    2011-10-27T01:27:23+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Common functions
 *
 *
 */
#include "Common.h"

#include <OGRE/Ogre.h>
#include <cstdio>
#include <cstdarg>

#include "CGame.h"

#if Linux
    #include <unistd.h>
#elseif Macos
    #include <CoreFoundation/CoreFoundation.h>
#elseif Windows
    #include <windows.h>
#endif

int Common::CLog::m_displayLevel = 0;
unsigned long Common::CLog::m_logTime = 0;

void Common::CLog::log(int level, const char* format, ...)
{
    // Message time
    m_logTime = CGame::getInstance()->getTime();

    char msgbuffer[CONFIG_LOG_BUFFER];
    char msglevel[6];
    std::va_list ap;

    // Select output
    FILE *output;
    if( level < 4 )
        output = stdout;
    else
        output = stderr;

    va_start(ap, format);
    std::vsnprintf(msgbuffer, CONFIG_LOG_BUFFER, format, ap);
    va_end(ap);

    // Message level
    switch( level ){
        case 1:
            strcpy(msglevel, "DEBUG");
            break;
        case 2:
            strcpy(msglevel, "INFO");
            break;
        case 3:
            strcpy(msglevel, "NOTICE");
            break;
        case 4:
            strcpy(msglevel, "WARN");
            break;
        case 5:
            strcpy(msglevel, "ERROR");
            break;
        case 6:
            strcpy(msglevel, "CRIT");
            break;
        case 7:
            strcpy(msglevel, "ALERT");
            break;
        case 8:
            strcpy(msglevel, "FATAL");
            break;
    }

    if( level > 0 )
        std::fprintf(output, "[%06d.%03d] %6s: %s\n", m_logTime/1000, m_logTime%1000, msglevel, msgbuffer);
}

int Common::CLog::displayLogLevel(int level)
{

}

std::string Common::getPrefixPath()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    char bin_path[1024];

    // Read symbolic link /proc/self/exe
    int len = readlink("/proc/self/exe", bin_path, sizeof(bin_path));
    if( len == -1 )
        throw new Ogre::Exception(0, "Can't find binary path", "");

    bin_path[len] = '\0';

    // Get the directory in the path by stripping exe name
    std::string out = std::string(bin_path);
    std::string prefix_bin("/" CONFIG_PATH_PREFIX_BIN "/");
    size_t found = out.rfind(prefix_bin);

    if( found == std::string::npos )
        throw new Ogre::Exception(0, "Can't find binary path", "");

    out = out.substr(0, found);

    return out;
    /// @todo complete getPrefixPath() for non-linux platforms
    /*
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    if(!mainBundle)
        throw new Ogre::Exception(0, "Can't find binary path", "");

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    if(!mainBundleURL)
        throw new Ogre::Exception(0, "Can't find binary path", "");

    CFStringRef cfStringRef = CFURLCopyFileSystemPath(mainBundleURL, kCFURLPOSIXPathStyle);
    if(!cfStringRef)
        throw new Ogre::Exception(0, "Can't find binary path", "");

    CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    TCHAR szPath[MAX_PATH];
    std::string execPath;
    std::string rootPath;
    if(GetModuleFileName(hInstance, szPath, MAX_PATH))
    {
        std::string path(szPath);
        size_t last = path.rfind('\\');
        if(last != std::string::npos)
        {
            execPath = path.substr(0, last+1);
            last = path.rfind('\\', last-1);
            if(last != std::string::npos)
                rootPath = path.substr(0, last+1);
        }
    }
    if(rootPath.empty())
        throw new Ogre::Exception(0, "Can't find binary path", "");

    return execPath;
    */
#endif
    throw new Ogre::Exception(0, "Can't find binary path", "");
}
