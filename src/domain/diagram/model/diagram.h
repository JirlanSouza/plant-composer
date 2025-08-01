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

#include "component_instance.h"
#include "connection.h"
#include <vector>

namespace diagram {
    class Diagram {
    public:
        Diagram(const std::string &id, const std::string &name);

        ~Diagram();

        void addComponent(const ComponentInstance &component);

        void addConnection(const Connection &connection);

        ComponentInstance *getLastAddedComponent();

        [[nodiscard]] int componentsCount() const;

        [[nodiscard]] int connectionsCount() const;

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] std::vector<ComponentInstance> getComponents() const;

        [[nodiscard]] std::vector<Connection> getConnections() const;

    private:
        std::string id_;
        std::string name_;
        int width_;
        int height_;
        std::vector<ComponentInstance> components_;
        std::vector<Connection> connections_;
    };
}
