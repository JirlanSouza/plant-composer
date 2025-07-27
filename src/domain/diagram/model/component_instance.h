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

#include <unordered_map>

#include "node_transform.h"
#include "domain/components_library/model/component_type.h"

namespace diagram {
    class ComponentInstance {
    public:
        ComponentInstance(
            std::string id,
            std::string name,
            std::string typeId,
            NodeTransform position,
            const components_library::ComponentType *component,
            std::string assetsPath
        );

        [[nodiscard]] const components_library::ComponentType *getDefinition() const { return component_; }

        [[nodiscard]] std::string getId() const { return id_; }

        [[nodiscard]] std::string getName() const { return name_; }

        [[nodiscard]] std::string getTypeId() const { return componentTypeId_; }

        [[nodiscard]] NodeTransform getPosition() const { return position_; }

        [[nodiscard]] std::string getSymbolPath() const { return assetsPath_ + "/" + component_->symbolPath; };


        void rename(const std::string &newName);

        void setPosition(const NodeTransform &newPosition);

        void setConstantValue(const std::string &name, const std::string &value);

    private:
        std::string id_;
        std::string name_;
        std::string componentTypeId_;
        NodeTransform position_;
        const components_library::ComponentType *component_;
        std::string assetsPath_;

        std::unordered_map<std::string, std::string> constants;
    };
}
