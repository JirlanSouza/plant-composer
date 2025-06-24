#pragma once
#include <QGraphicsSvgItem>

#include "chart_editor/model/component_instance.h"

namespace dce = domain::chart_editor;

namespace ui::chart_editor {
    class ComponentInstanceView final : public QGraphicsSvgItem {
        Q_OBJECT

    public:
        explicit ComponentInstanceView(QGraphicsItem *parent = nullptr,
                                       std::shared_ptr<dce::ComponentInstance> componentInstance = nullptr);

        ~ComponentInstanceView() override = default;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void setCenterPosition(int x, int y);

    protected
    :
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    private
    :
        std::shared_ptr<dce::ComponentInstance> componentInstance_;
    };
}
