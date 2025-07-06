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

#include "component_instance_factory.h"

#include <iostream>

namespace domain::diagram {
    ComponentInstanceFactory::ComponentInstanceFactory(
        IDFactory *idFactory,
        std::vector<dcl::Library> *libraries,
        dst::AppSettings *appSettings
    ): idFactory_(idFactory),
        libraries_(libraries),
        appSettings_(appSettings) {
    }

    ComponentInstance ComponentInstanceFactory::createComponentInstance(
        const int libraryId,
        const int componentId,
        NodeTransform position
    ) const {
        const auto componentDefinition = getComponentDefinition(libraryId, componentId);

        if (componentDefinition == nullptr) {
            throw std::runtime_error("Component definition not found.");
        }

        const auto id = idFactory_->newId();
        const std::string name = componentDefinition->name + "_" + id;
        const std::string assetsDir = appSettings_->getComponentsLibraryDir();

        return {
            id,
            name,
            componentDefinition->id,
            position,
            componentDefinition,
            assetsDir
        };
    }

    const dcl::ComponentType *ComponentInstanceFactory::getComponentDefinition(
        const int libraryId,
        const int componentId
    ) const {
        if (libraryId < 0 || libraryId >= static_cast<int>(libraries_->size())) {
            std::cerr << "Invalid libraryId: " << libraryId << std::endl;
            return nullptr;
        }

        const auto &library = libraries_->at(libraryId);

        if (componentId < 0 || componentId >= static_cast<int>(library.components.size())) {
            std::cerr << "Invalid componentId: " << componentId << std::endl;
            return nullptr;
        }

        return &library.components[componentId];
    }
}
