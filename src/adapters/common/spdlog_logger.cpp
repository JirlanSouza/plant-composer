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

namespace common {
    SpdlogLogger::SpdlogLogger(std::shared_ptr<spdlog::logger> logger) : logger_(std::move(logger)) {
    }

    void SpdlogLogger::log(const LogLevel level, const std::string &message) {
        switch (level) {
            case LogLevel::INFO: logger_->info(message);
                break;
            case LogLevel::DEBUG: logger_->debug(message);
                break;
            case LogLevel::WARN: logger_->warn(message);
                break;
            case LogLevel::ERROR: logger_->error(message);
                break;
            case LogLevel::FATAL: logger_->critical(message);
                break;
        }
    }
}
