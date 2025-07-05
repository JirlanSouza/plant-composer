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

#include "component_instance.h"

namespace domain::diagram {
    ComponentInstance::ComponentInstance(
        std::string id,
        std::string name,
        std::string typeId,
        NodeTransform position,
        const dcl::Component *component,
        std::string assetsPath
    ): id_(id),
        name_(name),
        componentTypeId_(typeId),
        position_(position),
        component_(component),
        assetsPath_(assetsPath) {
    }

    void ComponentInstance::rename(const std::string &newName) {
        name_ = newName;
    }

    void ComponentInstance::setPosition(const NodeTransform &newPosition) {
        position_ = newPosition;
    }


    void ComponentInstance::setConstantValue(const std::string &name, const std::string &value) {
        const auto it = std::ranges::find_if(
            component_->constants,
            [&](const dcl::Constant &constant) {
                return constant.name == name;
            }
        );

        if (it != component_->constants.end()) {
            constants[name] = value;
        }
    }
}
