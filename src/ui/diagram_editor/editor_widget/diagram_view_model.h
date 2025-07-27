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

#include <QObject>
#include <QList>

#include "domain/diagram/model/diagram.h"
#include "component_view_model.h"
#include "domain/diagram/component_instance_factory.h"

namespace diagram {
    class DiagramViewModel : public QObject {
        Q_OBJECT

    public:
        explicit DiagramViewModel(
            Diagram *diagram,
            ComponentInstanceFactory *componentInstanceFactory,
            QObject *parent = nullptr
        );

        ~DiagramViewModel() override;

        void addComponent(int libraryId, int componentId, QPointF posi);

        ComponentViewModel *getLastAddedComponentViewModel();

        [[nodiscard]] int componentsCount() const;

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

    signals:
        void componentAdded(diagram::ComponentViewModel *componentViewModel);

    private:
        Diagram *diagram_;
        ComponentInstanceFactory *componentInstanceFactory_;

        QList<ComponentViewModel *> componentViewModels_;
    };
}
