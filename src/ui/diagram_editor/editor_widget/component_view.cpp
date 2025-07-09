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

#include "component_view.h"

#include <QPainter>
#include <qpen.h>
#include <QSvgRenderer>

namespace ui::diagram_editor {
    ComponentView::ComponentView(
        ComponentViewModel *componentViewModel,
        QGraphicsItem *parent
    ): QGraphicsSvgItem(
            componentViewModel->getSymbolPath(),
            parent
        ),
        componentViewModel_(componentViewModel) {
        setFlags(
            QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemSendsScenePositionChanges
        );

        connect(
            componentViewModel_,
            &ComponentViewModel::positionChanged,
            this,
            [this]() {
                setPos(componentViewModel_->getPosition());
            }
        );

        setPos(componentViewModel_->getPosition());
    }

    void ComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        QGraphicsSvgItem::paint(painter, option, widget);

        if (isSelected()) {
            QPen pen(Qt::DashLine);
            pen.setColor(Qt::blue);
            pen.setWidth(0);
            painter->setPen(pen);
            painter->drawRect(boundingRect());
        }
    }

    void ComponentView::setCenterPosition(const int x, const int y) const {
        const auto xOffset = boundingRect().width() / 2;
        const auto yOffset = boundingRect().height() / 2;
        componentViewModel_->setPosition(QPointF(x - xOffset, y - yOffset));
    }

    void ComponentView::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        setSelected(true);
        update();
        QGraphicsSvgItem::mousePressEvent(event);
    }

    void ComponentView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsSvgItem::mouseReleaseEvent(event);
    }

    void ComponentView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsSvgItem::mouseDoubleClickEvent(event);
    }

    void ComponentView::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsSvgItem::mouseMoveEvent(event);
    }

    QVariant ComponentView::itemChange(const GraphicsItemChange change, const QVariant &value) {
        if (change == ItemPositionChange && scene() && !isPositionSameAsViewModel(value.toPointF())) {
            const QPointF newPos = value.toPointF();
            componentViewModel_->setPosition(newPos);
        }
        return QGraphicsSvgItem::itemChange(change, value);
    }

    bool ComponentView::isPositionSameAsViewModel(const QPointF &posi) const {
        return comparesEqual(componentViewModel_->getPosition(), posi);
    }
}
