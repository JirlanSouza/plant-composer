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
