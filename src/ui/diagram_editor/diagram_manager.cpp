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

namespace diagram {
    DiagramManager::DiagramManager(
        std::vector<components_library::Library> *libraries,
        settings::AppSettings *appSettings,
        ComponentInstanceFactory *componentInstanceFactory,
        project::ProjectViewModel *projectViewModel,
        QObject *parent
    ): QObject(parent),
        libraries_(libraries),
        openedDiagrams_(std::unordered_map<std::string, DiagramViewModel *>()),
        appSettings_(appSettings),
        componentInstanceFactory_(componentInstanceFactory),
        projectViewModel_(projectViewModel) {
        connect(projectViewModel_, &project::ProjectViewModel::openFileNode, this, &DiagramManager::openDiagram);
    }

    DiagramManager::~DiagramManager() = default;

    void DiagramManager::openDiagram(const project::FileNode *diagramMetadata) {
        if (openedDiagrams_.contains(diagramMetadata->getId())) {
            emit diagramOpened(diagramMetadata->getId());
            return;
        }

        const auto diagram = new Diagram(diagramMetadata->getId(), diagramMetadata->getName());
        openedDiagrams_[diagramMetadata->getId()] = new DiagramViewModel(diagram, componentInstanceFactory_, this);
        emit diagramOpened(diagramMetadata->getId());
    }

    DiagramViewModel *DiagramManager::getDiagram(const std::string &diagramId) {
        return openedDiagrams_[diagramId];
    }
}
