/**
 * @file    Common.h
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
#ifndef COMMON_H
#define COMMON_H

#include "config.h"

#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <iostream>

#include <string>
#include <vector>
#include <map>
#include <limits>

#include <libintl.h>
#include <clocale>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem3;

#include <OGRE/OgreTimer.h>

#ifdef CONFIG_DEBUG
#   include "OgreDebugDrawer/DebugDrawer.h"
#endif

#define _(string) gettext(string) ///< Gettext define

#ifdef CONFIG_DEBUG
#   define log_debug(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_DEBUG, format, ##__VA_ARGS__) ///< Debug log
#   define log_info(format, ...)    Common::CLog::log(Common::CLog::LogLevel::LOG_INFO, format, ##__VA_ARGS__)  ///< Info log
#   define EXCEPTION(message)       Common::Exception(message, __FUNCTION__, __FILE__, __LINE__)                ///< Hard exception
#   define ODD                      DebugDrawer::getSingleton()                                                 ///< Ogre Debug Drawer
#else
#   define log_debug(format, ...)   /* not logged */                         ///< Disabled Debug log
#   define log_info(format, ...)    /* not logged */                         ///< Disabled Info log
#   define EXCEPTION(message)       Common::Exception(message, __FUNCTION__) ///< Simple exception
#endif

#define log_notice(format, ...)  Common::CLog::log(Common::CLog::LogLevel::LOG_NOTICE, format, ##__VA_ARGS__) ///< Notice log
#define log_warn(format, ...)    Common::CLog::log(Common::CLog::LogLevel::LOG_WARN, format, ##__VA_ARGS__)   ///< Warning log
#define log_error(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_ERROR, format, ##__VA_ARGS__)  ///< Error log
#define log_crit(format, ...)    Common::CLog::log(Common::CLog::LogLevel::LOG_CRIT, format, ##__VA_ARGS__)   ///< Critical log
#define log_alert(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_ALERT, format, ##__VA_ARGS__)  ///< Alert log
#define log_emerg(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_EMERG, format, ##__VA_ARGS__)  ///< Emergency log


/** @brief Specialised and non-crossplatform functions
 */
namespace Common
{
    /** @brief Logging class
     */
    class CLog
    {
    public:
        /** @brief Log message level priority
         */
        enum LogLevel
        {
            LOG_NONE   = 0, ///< Won't be logged
            LOG_DEBUG  = 1, ///< Debug level
            LOG_INFO   = 2, ///< Trivial information
            LOG_NOTICE = 3, ///< Normal, but significant, condition
            LOG_WARN   = 4, ///< Warning conditions
            LOG_ERROR  = 5, ///< Error conditions
            LOG_CRIT   = 6, ///< Critical conditions
            LOG_ALERT  = 7, ///< Action must be taken immediately
            LOG_EMERG  = 8 ///< Game is unusable
        };

        /** @brief Log message
         *
         * @param level - Level of message
         * @param format - format of message like printf
         * @param ... - Message or printf format + parameters
         * @return bool - false, if level > 4
         *
         */
        static bool log(LogLevel level, const char* format, ...);

        /** @brief Get or set current displaying log level
         *
         * @param level - Set level of displaying message
         * @return LogLevel - Current displaying log level
         *
         */
        static LogLevel displayLogLevel(LogLevel level = LOG_NONE);

    protected:
        CLog();
        ~CLog();
        static LogLevel      s_DisplayLevel; ///< Display messages with >= this level
        static unsigned long s_LogTime;      ///< Last log time
        static Ogre::Timer   s_Timer;        ///< Logging timer
    };

    /** @brief Get prefix path from current binary path
     *
     * @return std::string
     *
     * Thanx Ogre wiki <http://www.ogre3d.org/tikiwiki/GetExecutablePath&structure=Cookbook>
     */
    std::string getPrefixPath();

    /** @brief A utility class to generate unique names
     *
     * Thanx Ogre forum <http://www.ogre3d.org/forums/viewtopic.php?f=2&t=58048>
     */
    class Name
    {
    public:
        /** @brief Gets the next unique name for a given prefix.
         */
        static std::string next(const std::string& prefix = "AutoGen");

        /** @brief Counts the number of names generated for a given prefix.
         *
         * @param prefix - The prefix of the generated names.
         */
        static ulong count(const std::string& prefix = "AutoGen") { return (s_NameCount.count(prefix) == 0) ? 0 : s_NameCount[prefix]; }

    private:
        Name() {  }
        ~Name() {  }

        typedef std::map<std::string, ulong> NameCountMap; ///< Name-Count map
        static NameCountMap s_NameCount;                   ///< Name-Count map instance
    };

    /** @brief Common simple Exception
     *
     * Usage:
     * @code
     * throw EXCEPTION("message"); //replaced to throw Common::Exception(message, __FUNC__ ...) and need constructor by CONFIG_DEBUG
     * @endcode
     */
    class Exception
        : public std::exception
    {
    public:
        /** @brief Default constructor
         *
         * @param description
         * @param source
         */
        Exception(const std::string& description, const std::string& source);

        /** @brief Extended constructor for CONFIG_DEBUG mode
         *
         * @param description
         * @param source
         * @param file
         * @param line
         */
        Exception(const std::string& description, const std::string& source, const char* file, long line);

        /** @brief Default destructor
         */
        ~Exception() throw() {}


        /** @brief Get formatted description with line number etc...
         *
         */
        const std::string& getFullDescription() const;

        /** @brief Get function name
         *
         * @return const std::string&
         */
        const std::string& getSource() const { return m_Source; }

        /** @brief Get file path
         *
         * @return const std::string&
         */
        const std::string& getFile() const { return m_File; }

        /** @brief Get line number
         *
         * @return long
         */
        long               getLine() const { return m_Line; }

        /** @brief Get description
         *
         * @return const std::string&
         */
        const std::string& getDescription() const { return m_Description; }

        /** @brief Override std::exception::what()
         *
         * @return const char *
         */
        const char*        what() const throw() { return getFullDescription().c_str(); }

    private:
        std::string          m_Source;          ///< Function name
        std::string          m_File;            ///< File path
        std::string          m_Description;     ///< Description message
        long                 m_Line;            ///< Line number
        mutable std::string  m_FullDescription; ///< Description message
    };

    /** @brief Stacktrace exception class for serious errors
     */
    class ExceptionTracer
    {
    public:
        ExceptionTracer()
        {
            void * array[25];
            int nSize = backtrace(array, 25);
            char** symbols = backtrace_symbols(array, nSize);

            for (int i = 0; i < nSize; i++)
                std::cout << symbols[i] << std::endl;

            free(symbols);
        }
        virtual ~ExceptionTracer() throw() {}
    };

    /** @brief Template of SignalExceptions
     *
     */
    template <class SignalExceptionClass> class SignalTranslator
    {
    private:
        class SingleTonTranslator
        {
        public:
            SingleTonTranslator()
            {
                signal(SignalExceptionClass::GetSignalNumber(), SignalHandler);
            }

            static void SignalHandler(int)
            {
                throw SignalExceptionClass();
            }
        };

    public:
        SignalTranslator()
        {
            static SingleTonTranslator s_objTranslator;
        }
    };

    /** @brief SIGSEGV signal catcher
     */
    class SegmentationFault
        : public ExceptionTracer
        , public std::exception
    {
    public:
        static int GetSignalNumber() {return SIGSEGV;}
    };

    /** @brief SIGFPE signal catcher
     */
    class FloatingPointException
        : public ExceptionTracer
        , public std::exception
    {
    public:
        static int GetSignalNumber() {return SIGFPE;}
    };
}

#endif // COMMON_H
