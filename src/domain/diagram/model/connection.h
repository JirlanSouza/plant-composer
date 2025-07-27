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

#include <string>

namespace diagram {
    class Connection {
    public:
        enum class Type {
            Port, Signal
        };

        Connection(
            const std::string &id,
            Type type,
            const std::string &sourceComponentId,
            const std::string &sourceIoId,
            const std::string &targetComponentId,
            const std::string &targetIoId
        );

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] Type getType() const;

        [[nodiscard]] std::string getSourceComponentId() const;

        [[nodiscard]] std::string getSourceIoId() const;

        [[nodiscard]] std::string getTargetComponentId() const;

        [[nodiscard]] std::string getTargetIoId() const;

    private:
        std::string id_;
        Type type_;
        std::string sourceComponentId_;
        std::string sourceIoId_;
        std::string targetComponentId_;
        std::string targetIoId_;
    };
}
