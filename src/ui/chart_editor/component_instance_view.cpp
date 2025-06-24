#include "component_instance_view.h"

#include <QPainter>
#include <qpen.h>
#include <QSvgRenderer>

namespace ui::chart_editor {
    ComponentInstanceView::ComponentInstanceView(QGraphicsItem *parent,
                                                 std::shared_ptr<dce::ComponentInstance>
                                                 componentInstance): QGraphicsSvgItem(
                                                                         QString::fromStdString(
                                                                             componentInstance->getSymbolPath()),
                                                                         parent),
                                                                     componentInstance_(componentInstance) {
        setFlags(QGraphicsItem::ItemIsSelectable |
                 QGraphicsItem::ItemIsMovable |
                 QGraphicsItem::ItemSendsScenePositionChanges);
    }

    void ComponentInstanceView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        QGraphicsSvgItem::paint(painter, option, widget);

        if (isSelected()) {
            QPen pen(Qt::DashLine);
            pen.setColor(Qt::blue);
            pen.setWidth(0.2);
            painter->setPen(pen);
            painter->drawRect(boundingRect());
        }
    }

    void ComponentInstanceView::setCenterPosition(const int x, const int y) {
        const auto xOffset = boundingRect().width() / 2;
        const auto yOffset = boundingRect().height() / 2;
        setPos(x - xOffset, y - yOffset);
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

    QVariant ComponentInstanceView::itemChange(GraphicsItemChange change, const QVariant &value) {
        if (change == ItemPositionChange) {
            QPointF newPos = value.toPointF();
            // limitar posição, ou emitir sinal de movimento
        }
        return QGraphicsSvgItem::itemChange(change, value);
    }
}
