#pragma once
#include <qevent.h>
#include <qgraphicsitem.h>
#include <QGraphicsView>

namespace ui::chart_editor {
    class ChartEditorView : public QGraphicsView {
        Q_OBJECT

    public:
        explicit ChartEditorView(QWidget *parent = nullptr);

    signals:
        void addComponentToScene(int libraryId, int componentId, QPointF posi);

    protected:
        void wheelEvent(QWheelEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void dragEnterEvent(QDragEnterEvent *event) override;

        void dragMoveEvent(QDragMoveEvent *event) override;

        void dropEvent(QDropEvent *event) override;

    private:
        QPoint lastPanPoint;
        bool panning = false;
    };
}
