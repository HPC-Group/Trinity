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

#ifndef __CONSOLELOG_H__
#define __CONSOLELOG_H__

#include "streamlog.h"

namespace ghoul
{
    namespace logging
    {
        /**
 * A concrete subclass of Log that logs the messages to the console using the
 * <code>std::cout</code> stream. The formatting of the log messages depends on the
 * stamping settings. The different possibilities are:
 * \verbatim
 "[DATE | TIME] CATEGORY (LEVEL) MESSAGE"
 "[DATE] CATEGORY (LEVEL) MESSAGE"
 "[TIME] CATEGORY (LEVEL) MESSAGE"
 \endverbatim
 * And the remaining possibilities with <code>CATEGORY</code> and <code>LEVEL</code>
 * missing. A parameter in the constructor determines if the output text will be colored
 * according to the LogManager::LogLevel:<br>
 * LogManager::LogLevel::Debug -> Green<br>
 * LogManager::LogLevel::Info -> Default color scheme of the console<br>
 * LogManager::LogLevel::Warning -> Yellow<br>
 * LogManager::LogLevel::Error -> Red<br>
 * LogManager::LogLevel::Fatal -> Cyan<br>
 */

        class ConsoleLog : public StreamLog
        {
        public:
            /**
    * Constructor that calls Log constructor.
    * \param colorOutput Determines if the log should printed in color
    * \param timeStamping Determines if the log should print the time when a message
    * is logged in the log messages
    * \param dateStamping Determines if the log should print the time when a message
    * is logged in the log messages
    * \param categoryStamping Determines if the log should print the categories in
    * the log messages
    * \param logLevelStamping Determines if the log should print the log level in the
    * log messages
    */
            ConsoleLog(bool colorOutput = true, bool timeStamping = false,
                       bool dateStamping = false, bool categoryStamping = true,
                       bool logLevelStamping = true);

            /**
     * Method that logs a message with a given level and category to the console.
     * \param level The log level with which the message shall be logged
     * \param category The category of this message. Can be used by each subclass
     * individually
     * \param message The message body of the log message
     */
            void log(LogManager::LogLevel level, const std::string& category,
                     const std::string& message) override;

        protected:
            // No defined on purpose. Using should throw linker error.
            ConsoleLog& operator=(const ConsoleLog&);
            ConsoleLog(const ConsoleLog&);
            /**
     * Prepares the console to print the next messages in the color according to the
     * LogManager::LogLevel:<br>
     * LogManager::LogLevel::Debug -> Green<br>
     * LogManager::LogLevel::Info -> Default color scheme of the console<br>
     * LogManager::LogLevel::Warning -> Yellow<br>
     * LogManager::LogLevel::Error -> Red<br>
     * LogManager::LogLevel::Fatal -> Cyan<br>
     * \param level The level that determines the color scheme for the console
     */
            void setColorForLevel(LogManager::LogLevel level);

            /**
     * Resets the console to the default color scheme (*nix) or the color scheme it had
     * when the program stared (Windows)
     */
            void resetColor();

            const bool _colorOutput; ///< Is the log printed in color?
        };
    } // namespace logging
} // namespace ghoul

#endif // __CONSOLELOG_H__

