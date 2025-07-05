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
#include <QGraphicsSvgItem>

#include "component_view_model.h"

namespace ui::diagram_editor {
    class ComponentInstanceView final : public QGraphicsSvgItem {
        Q_OBJECT

    public:
        explicit ComponentInstanceView(
            ComponentViewModel *componentViewModel,
            QGraphicsItem *parent
        );

        ~ComponentInstanceView() override = default;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void setCenterPosition(int x, int y) const;

    protected
    :
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    private
    :
        ComponentViewModel *componentViewModel_;
    };
}
