#include "component_instance_view.h"

#include <QPainter>
#include <qpen.h>
#include <QSvgRenderer>

namespace ui::diagram_editor {
    ComponentInstanceView::ComponentInstanceView(
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

    void ComponentInstanceView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        QGraphicsSvgItem::paint(painter, option, widget);

        if (isSelected()) {
            QPen pen(Qt::DashLine);
            pen.setColor(Qt::blue);
            pen.setWidth(0);
            painter->setPen(pen);
            painter->drawRect(boundingRect());
        }
    }

    void ComponentInstanceView::setCenterPosition(const int x, const int y) const {
        const auto xOffset = boundingRect().width() / 2;
        const auto yOffset = boundingRect().height() / 2;
        componentViewModel_->setPosition(QPointF(x - xOffset, y - yOffset));
    }

    void ComponentInstanceView::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        setSelected(true);
        update();
        QGraphicsSvgItem::mousePressEvent(event);
    }

    void ComponentInstanceView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsSvgItem::mouseReleaseEvent(event);
    }

    void ComponentInstanceView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsSvgItem::mouseDoubleClickEvent(event);
    }

    void ComponentInstanceView::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsSvgItem::mouseMoveEvent(event);
    }

    QVariant ComponentInstanceView::itemChange(const GraphicsItemChange change, const QVariant &value) {
        if (change == ItemPositionChange && scene()) {
            const QPointF newPos = value.toPointF();
            componentViewModel_->setPosition(newPos);
        }
        return QGraphicsSvgItem::itemChange(change, value);
    }
}
