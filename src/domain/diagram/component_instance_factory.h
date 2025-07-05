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
#include <vector>

#include "model/component_instance.h"
#include "model/node_transform.h"
#include "domain/components_library/model/library.h"
#include "domain/settings/app_settings.h"
#include "domain/shared/id_factory.h"

namespace dcl = domain::components_library;
namespace dst = domain::settings;

namespace domain::diagram {
    class ComponentInstanceFactory {
    public:
        explicit ComponentInstanceFactory(
            IDFactory *idFactory,
            std::vector<dcl::Library> *libraries,
            dst::AppSettings *appSettings
        );

        [[nodiscard]] ComponentInstance createComponentInstance(
            int libraryId,
            int componentId,
            NodeTransform position
        ) const;

    private:
        IDFactory *idFactory_;
        std::vector<dcl::Library> *libraries_;
        dst::AppSettings *appSettings_;

        [[nodiscard]] const dcl::Component *getComponentDefinition(
            int libraryId,
            int componentId
        ) const;
    };
}
