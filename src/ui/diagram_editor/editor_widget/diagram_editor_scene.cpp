#include "diagram_editor_scene.h"

namespace ui::diagram_editor {
    DiagramEditorScene::DiagramEditorScene(DiagramViewModel *diagramModel, QObject *parent) : QGraphicsScene(parent),
        diagramModel_(diagramModel), parent_(parent) {
        connect(diagramModel_, &DiagramViewModel::componentAdded, this, &DiagramEditorScene::onComponentAdded);
    }

    void DiagramEditorScene::drawBackground(QPainter *painter, const QRectF &rect) {
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

    void DiagramEditorScene::onComponentAdded(ComponentViewModel *componentViewModel) {
        if (componentViewModel == nullptr) {
            return;
        }
        const auto item = new ComponentInstanceView(componentViewModel, nullptr);
        componentInstanceViews_[componentViewModel->getId().toStdString()] = item;
        addItem(item);
    }
}
