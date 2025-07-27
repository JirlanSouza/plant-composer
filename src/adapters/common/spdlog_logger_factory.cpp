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

#include "spdlog_logger_factory.h"

#include "spdlog_logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace common {
    SpdlogLoggerFactory::SpdlogLoggerFactory() {
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] %^[%l]%$ \033[36m%-25n\033[0m: %v");
    }

    std::unique_ptr<Ilogger> SpdlogLoggerFactory::getLogger(const std::string &name) {
        auto spdlog_instance = spdlog::get(name);
        if (!spdlog_instance) {
            spdlog_instance = spdlog::stdout_color_mt(name);
        }
        return std::make_unique<SpdlogLogger>(spdlog_instance);
    }
}
