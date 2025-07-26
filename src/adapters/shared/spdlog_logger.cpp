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

#include "spdlog_logger.h"

#include <spdlog/spdlog.h>

namespace adapters {
    SpdlogLogger::SpdlogLogger() = default;

    SpdlogLogger::~SpdlogLogger() = default;

    void SpdlogLogger::log(const domain::LogLevel level, const std::string &message) {
        switch (level) {
            case domain::LogLevel::INFO: spdlog::info(message);
                break;
            case domain::LogLevel::DEBUG: spdlog::debug(message);
                break;
            case domain::LogLevel::WARN: spdlog::warn(message);
                break;
            case domain::LogLevel::ERROR: spdlog::error(message);
                break;
            case domain::LogLevel::FATAL: spdlog::critical(message);
                break;
        }
    }
}
