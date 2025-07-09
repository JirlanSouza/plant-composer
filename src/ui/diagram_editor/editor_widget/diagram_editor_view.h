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
#include <qevent.h>
#include <qgraphicsitem.h>
#include <QGraphicsView>

namespace ui::diagram_editor {
    class DiagramEditorView : public QGraphicsView {
        Q_OBJECT

    public:
        explicit DiagramEditorView(const std::string &diagramId, QWidget *parent = nullptr);

    signals:
        void addComponentToScene(std::string diagramId, int libraryId, int componentId, QPointF posi);

        public slots:
        void fitSceneToView();

    protected:
        void wheelEvent(QWheelEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void dragEnterEvent(QDragEnterEvent *event) override;

        void dragMoveEvent(QDragMoveEvent *event) override;

        void dropEvent(QDropEvent *event) override;

    private:
        std::string diagramId_;
        QPoint lastPanPoint;
        bool panning = false;
    };
}
