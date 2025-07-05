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

#include "diagram_view_model.h"

namespace ui::diagram_editor {
    DiagramViewModel::DiagramViewModel(dd::Diagram *diagram, QObject *parent): QObject(parent), diagram_(diagram) {
        for (auto component: diagram_->getComponents()) {
            const auto newComponentViewModel = new ComponentViewModel(&component, this);
            componentViewModels_.append(newComponentViewModel);
        }
    }

    DiagramViewModel::~DiagramViewModel() = default;

    void DiagramViewModel::addComponent(const dd::ComponentInstance &component) {
        diagram_->addComponent(component);

        const auto newComponentViewModel = new ComponentViewModel(diagram_->getLastAddedComponent(), this);
        componentViewModels_.append(newComponentViewModel);
        emit componentAdded(newComponentViewModel);
    }

    ComponentViewModel *DiagramViewModel::getLastAddedComponentViewModel() {
        if (componentViewModels_.isEmpty()) {
            return nullptr;
        }
        return componentViewModels_.last();
    }

    int DiagramViewModel::componentsCount() const {
        return diagram_->componentsCount();
    }

    std::string DiagramViewModel::getId() const {
        return diagram_->getId();
    }

    std::string DiagramViewModel::getName() const {
        return diagram_->getName();
    }

    int DiagramViewModel::getWidth() const {
        return diagram_->getWidth();
    }

    int DiagramViewModel::getHeight() const {
        return diagram_->getHeight();
    }
}
