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
#include "id/id_utils.h"

namespace ui::diagram_editor {
    DiagramManager::DiagramManager(
        std::vector<dcl::Library> *libraries,
        dst::AppSettings *appSettings,
        dd::ComponentInstanceFactory *componentInstanceFactory,
        QObject *parent
    ): QObject(parent),
        libraries_(libraries),
        openedDiagrams_(std::unordered_map<std::string, DiagramViewModel *>()),
        appSettings_(appSettings),
        componentInstanceFactory_(componentInstanceFactory) {
    }

    DiagramManager::~DiagramManager() = default;


    void DiagramManager::openDiagram(const std::string &diagramId) {
        if (openedDiagrams_.contains(diagramId)) {
            emit diagramOpened(diagramId);
            return;
        }

        const auto diagram = new dd::Diagram(diagramId, "Diagram_" + std::to_string(openedDiagrams_.size()));

        openedDiagrams_[diagramId] = new DiagramViewModel(diagram, componentInstanceFactory_, this);

        emit diagramOpened(diagramId);
    }


    DiagramViewModel *DiagramManager::getDiagram(const std::string &diagramId) {
        return openedDiagrams_[diagramId];
    }
}
