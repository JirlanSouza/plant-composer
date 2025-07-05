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

#include "diagram_manager.h"

#include <iostream>
#include <qpoint.h>
#include "id/id_utils.h"

namespace ui::diagram_editor {
    DiagramManager::DiagramManager(
        std::vector<dcl::Library> *libraries,
        dst::AppSettings *appSettings,
        QObject *parent
    ): QObject(parent),
        libraries_(libraries),

        openedDiagrams_(std::unordered_map<std::string, DiagramViewModel *>()),
        appSettings_(appSettings) {
    }

    DiagramManager::~DiagramManager() = default;


    void DiagramManager::openDiagram(const std::string &diagramId) {
        if (openedDiagrams_.contains(diagramId)) {
            emit diagramOpened(diagramId);
            return;
        }

        const auto diagram = new dd::Diagram(diagramId, "Diagram_" + std::to_string(openedDiagrams_.size()));

        openedDiagrams_[diagramId] = new DiagramViewModel(diagram, this);

        emit diagramOpened(diagramId);
    }


    DiagramViewModel *DiagramManager::getDiagram(const std::string &diagramId) {
        return openedDiagrams_[diagramId];
    }


    void DiagramManager::addComponent(
        const std::string &diagramId,
        const int libraryId,
        const int componentId,
        const QPointF posi
    ) const {
        if (!openedDiagrams_.contains(diagramId)) {
            return;
        }


        DiagramViewModel *diagram = openedDiagrams_.at(diagramId);

        std::cout << libraries_->size() << std::endl;

        const auto component = getComponent(libraryId, componentId);

        if (component == nullptr) {
            return;
        }

        auto position = dd::NodeTransform();
        position.x = posi.x();
        position.y = posi.y();

        const std::string name = component->name + "_" + std::to_string(diagram->componentsCount());
        const std::string librariesDir = appSettings_->getComponentsLibraryDir();

        const auto componentInstance = dd::ComponentInstance(
            newId(),
            name,
            component->id,
            position,
            component,
            librariesDir
        );

        diagram->addComponent(componentInstance);
    }

    const dcl::Component *DiagramManager::getComponent(const int libraryId, const int componentId) const {
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
