#include "chart_editor_scene.h"

#include <iostream>

namespace ui::chart_editor {
    ChartEditorScene::ChartEditorScene(QObject *parent) : QGraphicsScene(parent), parent_(parent) {
    }

    void ChartEditorScene::drawBackground(QPainter *painter, const QRectF &rect) {
        painter->setPen(QPen(gridColor, 0.4, Qt::DashLine));

        const int left = static_cast<int>(std::floor(rect.left()));
        const int right = static_cast<int>(std::ceil(rect.right()));
        const int top = static_cast<int>(std::floor(rect.top()));
        const int bottom = static_cast<int>(std::ceil(rect.bottom()));

        for (int x = left - (left % gridSize); x < right; x += gridSize) {
            painter->drawLine(x, top, x, bottom);
        }

        for (int y = top - (top % gridSize); y < bottom; y += gridSize) {
            painter->drawLine(left, y, right, y);
        }
    }

    void ChartEditorScene::onComponentAdded(const std::shared_ptr<dce::ComponentInstance> &componentInstance) {
        if (componentInstance == nullptr) {
            return;
        }

        componentInstances_[componentInstance->getId()] = componentInstance;
        const auto item = new ComponentInstanceView(nullptr, componentInstance);
        const auto position = componentInstance->getPosition();
        item->setCenterPosition(position.x, position.y);
        componentInstanceViews_[componentInstance->getId()] = item;
        addItem(item);
    }
}
