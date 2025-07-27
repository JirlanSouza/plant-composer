/*
 * plant-composer
 * Copyright (c) 2025 jirlansouza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <format>
#include <string>

namespace common {
    enum class LogLevel {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    class Ilogger {
    public:
        virtual ~Ilogger() = default;

        virtual void log(LogLevel level, const std::string &message) = 0;

        template<typename... Args>
        void debug(std::format_string<Args...> fmt, Args &&... args) {
            log(LogLevel::DEBUG, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void info(std::format_string<Args...> fmt, Args &&... args) {
            log(LogLevel::INFO, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void warn(std::format_string<Args...> fmt, Args &&... args) {
            log(LogLevel::WARN, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void error(std::format_string<Args...> fmt, Args &&... args) {
            log(LogLevel::ERROR, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void fatal(std::format_string<Args...> fmt, Args &&... args) {
            log(LogLevel::FATAL, std::format(fmt, std::forward<Args>(args)...));
        }
    };
}
