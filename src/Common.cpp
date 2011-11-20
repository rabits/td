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

using namespace Common;

CLog::LogLevel CLog::s_displayLevel = CLog::LOG_NONE;
unsigned long CLog::s_logTime = 0;
Ogre::Timer CLog::s_Timer;

bool CLog::log(CLog::LogLevel level, const char* format, ...)
{
    if( level < s_displayLevel )
        return level < 5;

    // Message time
    s_logTime = s_Timer.getMilliseconds();

    char msgbuffer[CONFIG_LOG_BUFFER];
    char msglevel[] = "       ";
    std::va_list ap;

    // Select output
    FILE* output;
    if( level < CLog::LOG_WARN )
        output = stdout;
    else
        output = stderr;

    va_start(ap, format);
    std::vsnprintf(msgbuffer, CONFIG_LOG_BUFFER, format, ap);
    va_end(ap);

    // Message level
    switch( level ){
        case 1:
            std::strcpy(msglevel, "DEBUG");
            break;
        case 2:
            std::strcpy(msglevel, "INFO");
            break;
        case 3:
            std::strcpy(msglevel, "NOTICE");
            break;
        case 4:
            std::strcpy(msglevel, "WARN");
            break;
        case 5:
            std::strcpy(msglevel, "ERROR");
            break;
        case 6:
            std::strcpy(msglevel, "CRIT");
            break;
        case 7:
            std::strcpy(msglevel, "ALERT");
            break;
        case 8:
            std::strcpy(msglevel, "FATAL");
            break;
        case 0:
            return true;
    }

    std::fprintf(output, "[%06lu.%03lu] %6s: %s\n", s_logTime/1000, s_logTime%1000, msglevel, msgbuffer);

    return level < CLog::LOG_ERROR;
}

CLog::LogLevel CLog::displayLogLevel(CLog::LogLevel level)
{
    if( level != CLog::LOG_NONE )
        s_displayLevel = level;

    return s_displayLevel;
}

std::string Common::getPrefixPath()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    char bin_path[1024];

    // Read symbolic link /proc/self/exe
    int len = readlink("/proc/self/exe", bin_path, sizeof(bin_path));
    if( len == -1 )
        throw EXCEPTION("Can't find binary path");

    bin_path[len] = '\0';

    // Get the directory in the path by stripping exe name
    std::string out = std::string(bin_path);
    std::string prefix_bin("/" CONFIG_PATH_PREFIX_BIN "/");
    size_t found = out.rfind(prefix_bin);

    if( found == std::string::npos )
        throw EXCEPTION("Can't find binary path");

    out = out.substr(0, found);

    return out;
    /// @todo complete getPrefixPath() for non-linux platforms
    /*
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    if(!mainBundle)
        throw EXCEPTION("Can't find binary path");

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    if(!mainBundleURL)
        throw EXCEPTION("Can't find binary path");

    CFStringRef cfStringRef = CFURLCopyFileSystemPath(mainBundleURL, kCFURLPOSIXPathStyle);
    if(!cfStringRef)
        throw EXCEPTION("Can't find binary path");

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
        throw EXCEPTION("Can't find binary path");

    return execPath;
    */
#endif
    throw EXCEPTION("Can't find binary path");
}

Exception::Exception(const std::string& description, const std::string& source)
    : m_Source(source)
    , m_File()
    , m_Description(description)
    , m_Line(0)
    , m_FullDescription()
{
}

Exception::Exception(const std::string& description, const std::string& source, const char* file, long line)
    : m_Source(source)
    , m_File(file)
    , m_Description(description)
    , m_Line(line)
    , m_FullDescription()
{
}

const std::string& Exception::getFullDescription() const
{
    if( m_FullDescription.empty() )
    {
        std::stringstream full;

        full << "Exception: '" << m_Description << "' in " << m_Source;

        if( m_Line > 0 )
            full << " at " << m_File << "(line " << m_Line << ")";

        m_FullDescription = full.str();
    }

    return m_FullDescription;
}
