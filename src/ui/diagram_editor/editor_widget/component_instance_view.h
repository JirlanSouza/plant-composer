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
