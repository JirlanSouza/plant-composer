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

#include "component_view_model.h"

namespace ui::diagram_editor {
    ComponentViewModel::ComponentViewModel(dd::ComponentInstance *componentInstance, QObject *parent)
        : QObject(parent), componentInstance_(componentInstance) {
    }

    QString ComponentViewModel::getId() const {
        return QString::fromStdString(componentInstance_->getId());
    }

    QString ComponentViewModel::getName() const {
        return QString::fromStdString(componentInstance_->getName());
    }

    QString ComponentViewModel::getSymbolPath() const {
        return QString::fromStdString(componentInstance_->getSymbolPath());
    }

    QPointF ComponentViewModel::getPosition() const {
        const auto &pos = componentInstance_->getPosition();
        return QPointF(pos.x, pos.y);
    }

    void ComponentViewModel::setPosition(const QPointF &newPosition) {
        auto pos = componentInstance_->getPosition();
        pos.x = static_cast<int>(newPosition.x());
        pos.y = static_cast<int>(newPosition.y());
        componentInstance_->setPosition(pos);

        emit positionChanged();
    }
}
