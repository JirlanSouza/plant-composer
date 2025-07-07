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
#include <QPointF>
#include <QString>

#include "domain/diagram/model/component_instance.h"

namespace dd = domain::diagram;

namespace ui::diagram_editor {
    class ComponentViewModel : public QObject {
        Q_OBJECT

    public:
        explicit ComponentViewModel(dd::ComponentInstance *componentInstance, QObject *parent = nullptr);

        ~ComponentViewModel() override = default;

        [[nodiscard]] QString getId() const;

        [[nodiscard]] QString getName() const;

        void setName(const QString &name);

        [[nodiscard]] QString getSymbolPath() const;

        [[nodiscard]] QPointF getPosition() const;

        void setPosition(const QPointF &newPosition);

        [[nodiscard]] dd::ComponentInstance *getDomainComponent() const { return componentInstance_; }

    signals:
        void nameChanged();

        void positionChanged();

    private:
        dd::ComponentInstance *componentInstance_;
    };
}
