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

// @todo Fix strange memory bug with this defines (notice-emerg). If its move from ifdef - will be produced sigfault...
#ifdef CONFIG_DEBUG
#   define log_debug(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_DEBUG, format, ##__VA_ARGS__)
#   define log_info(format, ...)    Common::CLog::log(Common::CLog::LogLevel::LOG_INFO, format, ##__VA_ARGS__)
#else
#   define log_debug(format, ...)  /* not logged */
#   define log_info(format, ...)   /* not logged */
#endif

#define log_notice(format, ...)  Common::CLog::log(Common::CLog::LogLevel::LOG_NOTICE, format, ##__VA_ARGS__)
#define log_warn(format, ...)    Common::CLog::log(Common::CLog::LogLevel::LOG_WARN, format, ##__VA_ARGS__)
#define log_error(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_ERROR, format, ##__VA_ARGS__)
#define log_crit(format, ...)    Common::CLog::log(Common::CLog::LogLevel::LOG_CRIT, format, ##__VA_ARGS__)
#define log_alert(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_ALERT, format, ##__VA_ARGS__)
#define log_emerg(format, ...)   Common::CLog::log(Common::CLog::LogLevel::LOG_EMERG, format, ##__VA_ARGS__)

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
         * @param level LogLevel - Level of message
         * @param ... - Message or printf format + parameters
         * @return bool - false, if level > 4
         *
         */
        static bool log(LogLevel level, const char* format, ...);

        /** @brief Get or set current displaying log level
         *
         * @param level LogLevel - Set level of displaying message
         * @return LogLevel - Current displaying log level
         *
         */
        static LogLevel displayLogLevel(LogLevel level = LOG_NONE);

    protected:
        static LogLevel      m_displayLevel; ///< Display messages with >= this level
        static unsigned long m_logTime; ///< Last log time
    };

    /** @brief Get prefix path from current binary path
     *
     * @return std::string
     *
     * Thanx Ogre wiki <http://www.ogre3d.org/tikiwiki/GetExecutablePath&structure=Cookbook>
     */
    std::string getPrefixPath();

    /** @brief Stacktrace exception class
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
            {
                std::cout << symbols[i] << std::endl;
            }

            free(symbols);
        }
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
    class SegmentationFault : public ExceptionTracer, public std::exception
    {
    public:
        static int GetSignalNumber() {return SIGSEGV;}
    };

    /** @brief SIGFPE signal catcher
     */
    class FloatingPointException : public ExceptionTracer, public std::exception
    {
    public:
        static int GetSignalNumber() {return SIGFPE;}
    };
}

#endif // COMMON_H
