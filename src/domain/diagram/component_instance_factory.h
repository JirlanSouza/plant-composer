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

#include <vector>

#include "domain/common/id_factory.h"
#include "domain/settings/app_settings.h"
#include "model/component_instance.h"
#include "model/node_transform.h"
#include "domain/components_library/model/library.h"

namespace diagram {
    class ComponentInstanceFactory {
    public:
        explicit ComponentInstanceFactory(
            common::IDFactory *idFactory,
            std::vector<components_library::Library> *libraries,
            settings::AppSettings *appSettings
        );

        [[nodiscard]] ComponentInstance createComponentInstance(
            int libraryId,
            int componentId,
            NodeTransform position
        ) const;

    private:
        common::IDFactory *idFactory_;
        std::vector<components_library::Library> *libraries_;
        settings::AppSettings *appSettings_;

        [[nodiscard]] const components_library::ComponentType *getComponentDefinition(
            int libraryId,
            int componentId
        ) const;
    };
}
