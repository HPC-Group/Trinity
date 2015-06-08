/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2014                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#include "logging/logmanager.h"
#include "logging/log.h"
#include <cassert>
#include <algorithm>
#include <atomic>
#include <vector>

namespace ghoul
{
    namespace logging
    {
        LogManager* LogManager::_manager = nullptr;

        /// \cond DEV
        /**
 * The LogManager_P is the private version of the LogManager and uses template
 * instantiation to increase the performance of the logging. Specifying the
 * LogManager::LogLevel as a template argument allows the compiler to optimize the check
 * if a passed message should be relayed or not.
 * The same argument holds for the IMMEDIATEFLUSH that is used to control if the
 * Log::flush() methods should be called after each log(LogManager::LogLevel, std::string,
 * std::string) call to the Logs. After template argument insertion, the appropriate
 * checks will be class-constant and can therefore be optimized.
 * The same technique allows the ignoring of the Debug messages in a Release build.
 * The sole cost of this is a virtual function call in the log message.
 * \tparam LEVEL Determines the lowest level of LogMessages this LogManager accepts
 * \tparam IMMEDIATEFLUSH Controls if the Log::flush() message should be called after each
 * log message.
 */
        template <LogManager::LogLevel LEVEL, bool IMMEDIATEFLUSH>
        class LogManager_P : public LogManager
        {
        public:
            LogManager_P() : LogManager()
            {
                _mutex.clear();
            }

            void logMessage(LogLevel level, const std::string& category,
                            const std::string& message);
        private:
            std::atomic_flag _mutex;
        };

        template <LogManager::LogLevel LEVEL, bool IMMEDIATEFLUSH>
        void LogManager_P<LEVEL, IMMEDIATEFLUSH>::logMessage(LogManager::LogLevel level,
                                                             const std::string& category,
                                                             const std::string& message)
        {
            if (level >= LEVEL)
            {
                // Acquire lock
                while (_mutex.test_and_set())
                {}

                std::vector<Log*>::const_iterator it = _logs.begin();
                for (; it != _logs.end(); ++it)
                {
                    (*it)->log(level, category, message);
                    if (IMMEDIATEFLUSH)
                        (*it)->flush();
                }

                _mutex.clear();
            }
        }

        /// \endcond

        std::string LogManager::stringFromLevel(LogLevel level)
        {
            switch (level)
            {
            case LogLevel::Debug:
                return "Debug";
            case LogLevel::Info:
                return "Info";
            case LogLevel::Warning:
                return "Warning";
            case LogLevel::Error:
                return "Error";
            case LogLevel::Fatal:
                return "Fatal";
            case LogLevel::None:
                return "None";
            }
            assert(false);
            return "";
        }

        void LogManager::initialize(LogManager::LogLevel level, bool immediateFlush)
        {
            assert(_manager == nullptr);
            switch (level)
            {
            case LogLevel::Debug:
                if (immediateFlush)
                    _manager = new LogManager_P<LogLevel::Debug, true>();
                else
                    _manager = new LogManager_P<LogLevel::Debug, false>();
                break;
            case LogLevel::Info:
                if (immediateFlush)
                    _manager = new LogManager_P<LogLevel::Info, true>();
                else
                    _manager = new LogManager_P<LogLevel::Info, false>();
                break;
            case LogLevel::Warning:
                if (immediateFlush)
                    _manager = new LogManager_P<LogLevel::Warning, true>();
                else
                    _manager = new LogManager_P<LogLevel::Warning, false>();
                break;
            case LogLevel::Error:
                if (immediateFlush)
                    _manager = new LogManager_P<LogLevel::Error, true>();
                else
                    _manager = new LogManager_P<LogLevel::Error, false>();
                break;
            case LogLevel::Fatal:
                if (immediateFlush)
                    _manager = new LogManager_P<LogLevel::Fatal, true>();
                else
                    _manager = new LogManager_P<LogLevel::Fatal, false>();
                break;
            case LogLevel::None:
                if (immediateFlush)
                    _manager = new LogManager_P<LogLevel::None, true>();
                else
                    _manager = new LogManager_P<LogLevel::None, false>();
                break;
            }
            assert(_manager != 0);
        }

        void LogManager::deinitialize()
        {
            assert(_manager != nullptr);
            delete _manager;
            _manager = nullptr;
        }

        LogManager::~LogManager()
        {
            for (std::vector<Log*>::iterator it = _logs.begin(); it != _logs.end(); ++it)
                delete (*it);
            _logs.clear();
        }

        LogManager& LogManager::ref()
        {
            assert(_manager != nullptr);
            return *_manager;
        }

        bool LogManager::isInitialized()
        {
            return (_manager != nullptr);
        }

        void LogManager::addLog(Log* log)
        {
            std::vector<Log*>::const_iterator it = std::find(_logs.begin(), _logs.end(), log);
            if (it == _logs.end())
                _logs.push_back(log);
        }

        void LogManager::removeLog(Log* log)
        {
            std::vector<Log*>::iterator it = std::find(_logs.begin(), _logs.end(), log);
            if (it != _logs.end())
                _logs.erase(it);
        }

        void LogManager::logMessage(LogManager::LogLevel level, const std::string& message)
        {
            _manager->logMessage(level, "", message);
        }
    } // namespace logging
} // namespace ghoul

