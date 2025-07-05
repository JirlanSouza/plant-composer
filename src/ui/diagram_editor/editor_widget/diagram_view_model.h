/*
 * plant-composer
 * Copyright (C) 2025 Jirlan Souza
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

namespace dd = domain::diagram;

namespace ui::diagram_editor {
    class DiagramViewModel : public QObject {
        Q_OBJECT

    public:
        explicit DiagramViewModel(dd::Diagram *diagram, QObject *parent = nullptr);

        ~DiagramViewModel() override;

        void addComponent(const dd::ComponentInstance &component);

        ComponentViewModel *getLastAddedComponentViewModel();

        [[nodiscard]] int componentsCount() const;

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

    signals:
        void componentAdded(ui::diagram_editor::ComponentViewModel *componentViewModel);

    private:
        dd::Diagram *diagram_;

        QList<ComponentViewModel *> componentViewModels_;
    };
}
