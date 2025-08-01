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

#include "diagram_editor_scene.h"

namespace diagram {
    DiagramEditorScene::DiagramEditorScene(DiagramViewModel *diagramModel, QObject *parent) : QGraphicsScene(parent),
        diagramModel_(diagramModel), parent_(parent) {
        setSceneRect(0, 0, diagramModel->getWidth(), diagramModel->getHeight());
        connect(diagramModel_, &DiagramViewModel::componentAdded, this, &DiagramEditorScene::onComponentAdded);
    }

    void DiagramEditorScene::drawBackground(QPainter *painter, const QRectF &_) {
        painter->setPen(QPen(gridColor, 0.4, Qt::DashLine));
        const auto rect = sceneRect();
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

        painter->setPen(QPen(QColor(240, 240, 240), 2, Qt::SolidLine));
        painter->drawRect(rect);
    }

    void DiagramEditorScene::onComponentAdded(ComponentViewModel *componentViewModel) {
        if (componentViewModel == nullptr) {
            return;
        }
        const auto item = new ComponentView(componentViewModel, nullptr);
        componentInstanceViews_[componentViewModel->getId().toStdString()] = item;
        addItem(item);
    }
}
