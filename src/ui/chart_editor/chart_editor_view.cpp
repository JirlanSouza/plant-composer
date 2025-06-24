#include "chart_editor_view.h"

#include <QApplication>
#include <QMimeData>
#include <QMouseEvent>
#include <QScrollBar>

#include "mime_types.h"

namespace ui::chart_editor {
    ChartEditorView::ChartEditorView(QWidget *parent)
        : QGraphicsView(parent) {
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::RubberBandDrag);
        setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setResizeAnchor(QGraphicsView::AnchorViewCenter);

        setAcceptDrops(true);
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    // Zoom com roda do mouse
    void ChartEditorView::wheelEvent(QWheelEvent *event) {
        const double zoomFactor = 1.15;
        if (event->angleDelta().y() > 0) {
            scale(zoomFactor, zoomFactor);
        } else {
            scale(1.0 / zoomFactor, 1.0 / zoomFactor);
        }
    }

    // Pan com botão do meio do mouse
    void ChartEditorView::mousePressEvent(QMouseEvent *event) {
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

    void ChartEditorView::mouseMoveEvent(QMouseEvent *event) {
        if (panning) {
            QPoint delta = event->pos() - lastPanPoint;
            lastPanPoint = event->pos();
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            event->accept();
        } else {
            QGraphicsView::mouseMoveEvent(event);
        }
    }

    void ChartEditorView::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::MiddleButton || (
                event->button() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::Key_Space)) {
            panning = false;
            setCursor(Qt::ArrowCursor);
            event->accept();
        } else {
            QGraphicsView::mouseReleaseEvent(event);
        }
    }

    void ChartEditorView::dragEnterEvent(QDragEnterEvent *event) {
        if (event->mimeData()->hasFormat(MIME_TYPE_COMPONENT)) {
            event->acceptProposedAction();
        }
    }

    void ChartEditorView::dragMoveEvent(QDragMoveEvent *event) {
        event->acceptProposedAction();
    }

    void ChartEditorView::dropEvent(QDropEvent *event) {
        if (!scene()) return;

        QByteArray data = event->mimeData()->data(MIME_TYPE_COMPONENT);
        QDataStream dataStream(&data, QIODevice::ReadOnly);
        int libraryIndex;
        int componentIndex;
        dataStream >> libraryIndex >> componentIndex;


        const QPointF scenePos = mapToScene(event->position().toPoint());

        emit addComponentToScene(libraryIndex, componentIndex, scenePos);

        event->acceptProposedAction();
    }
}
