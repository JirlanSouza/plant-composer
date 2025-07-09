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

#include "diagram_editor_view.h"

#include <QApplication>
#include <QMimeData>
#include <QMouseEvent>
#include <QScrollBar>

#include "mime_types.h"

namespace ui::diagram_editor {
    DiagramEditorView::DiagramEditorView(const std::string &diagramId, QWidget *parent)
        : QGraphicsView(parent), diagramId_(diagramId) {
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::RubberBandDrag);
        setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setResizeAnchor(QGraphicsView::AnchorViewCenter);

        setAcceptDrops(true);
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    void DiagramEditorView::fitSceneToView() {
        if (!scene()) {
            return;
        }

        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }


    void DiagramEditorView::wheelEvent(QWheelEvent *event) {
        constexpr double zoomFactor = 1.15;
        if (event->angleDelta().y() > 0) {
            scale(zoomFactor, zoomFactor);
        } else {
            scale(1.0 / zoomFactor, 1.0 / zoomFactor);
        }
    }


    void DiagramEditorView::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::MiddleButton || (
                event->button() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::Key_Space)) {
            panning = true;
            lastPanPoint = event->pos();
            setCursor(Qt::ClosedHandCursor);
            event->accept();
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    }

    void DiagramEditorView::mouseMoveEvent(QMouseEvent *event) {
        if (panning) {
            const QPoint delta = event->pos() - lastPanPoint;
            lastPanPoint = event->pos();
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            event->accept();
        } else {
            QGraphicsView::mouseMoveEvent(event);
        }
    }

    void DiagramEditorView::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::MiddleButton || (
                event->button() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::Key_Space)) {
            panning = false;
            setCursor(Qt::ArrowCursor);
            event->accept();
        } else {
            QGraphicsView::mouseReleaseEvent(event);
        }
    }

    void DiagramEditorView::dragEnterEvent(QDragEnterEvent *event) {
        if (event->mimeData()->hasFormat(MIME_TYPE_COMPONENT)) {
            event->acceptProposedAction();
        }
    }

    void DiagramEditorView::dragMoveEvent(QDragMoveEvent *event) {
        event->acceptProposedAction();
    }

    void DiagramEditorView::dropEvent(QDropEvent *event) {
        if (!scene()) return;

        QByteArray data = event->mimeData()->data(MIME_TYPE_COMPONENT);
        QDataStream dataStream(&data, QIODevice::ReadOnly);
        int libraryIndex;
        int componentIndex;
        dataStream >> libraryIndex >> componentIndex;


        const QPointF scenePos = mapToScene(event->position().toPoint());

        emit addComponentToScene(diagramId_, libraryIndex, componentIndex, scenePos);

        event->acceptProposedAction();
    }
}
