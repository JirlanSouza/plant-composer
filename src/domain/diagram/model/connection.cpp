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
 * along with this program.  If not, see <https://www.gnu.nu/licenses/>.
 */

#include "connection.h"

namespace diagram {
    Connection::Connection(
        const std::string &id,
        const Type type,
        const std::string &sourceComponentId,
        const std::string &sourceIoId,
        const std::string &targetComponentId,
        const std::string &targetIoId
    )
        : id_(id),
        type_(type),
        sourceComponentId_(sourceComponentId),
        sourceIoId_(sourceIoId),
        targetComponentId_(targetComponentId),
        targetIoId_(targetIoId) {
    }

    std::string Connection::getId() const {
        return id_;
    }

    Connection::Type Connection::getType() const {
        return type_;
    }

    std::string Connection::getSourceComponentId() const {
        return sourceComponentId_;
    }

    std::string Connection::getSourceIoId() const {
        return sourceIoId_;
    }

    std::string Connection::getTargetComponentId() const {
        return targetComponentId_;
    }

    std::string Connection::getTargetIoId() const {
        return targetIoId_;
    }
}
