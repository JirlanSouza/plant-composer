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

#include "diagram.h"

namespace diagram {
    Diagram::Diagram(const std::string &id, const std::string &name): id_(id), name_(name), width_(1920), height_(1080),
        components_(std::vector<ComponentInstance>()), connections_(std::vector<Connection>()) {
    };

    Diagram::~Diagram() = default;


    void Diagram::addComponent(const ComponentInstance &component) {
        components_.emplace_back(component);
    }

    void Diagram::addConnection(const Connection &connection) {
        connections_.emplace_back(connection);
    }

    ComponentInstance *Diagram::getLastAddedComponent() {
        return &components_[components_.size() - 1];
    }


    int Diagram::componentsCount() const {
        return static_cast<int>(components_.size());
    }

    int Diagram::connectionsCount() const {
        return static_cast<int>(connections_.size());
    }

    std::string Diagram::getId() const {
        return id_;
    }

    std::string Diagram::getName() const {
        return name_;
    }


    int Diagram::getWidth() const {
        return width_;
    }

    int Diagram::getHeight() const {
        return height_;
    }

    std::vector<ComponentInstance> Diagram::getComponents() const {
        return components_;
    }

    std::vector<Connection> Diagram::getConnections() const {
        return connections_;
    }
}
