#pragma once
#include <QGraphicsScene>
#include <QPainter>

#include "diagram_view_model.h"
#include "component_instance_view.h"
#include "component_view_model.h"

namespace ui::diagram_editor {
    class DiagramEditorScene final : public QGraphicsScene {
        Q_OBJECT

    public:
        explicit DiagramEditorScene(DiagramViewModel *diagramModel, QObject *parent = nullptr);

        ~DiagramEditorScene() override = default;

    protected:
        void drawBackground(QPainter *painter, const QRectF &rect) override;

    private:
        DiagramViewModel *diagramModel_;
        QObject *parent_;
        const int gridSize = 50;
        const QColor gridColor = QColor(220, 220, 220);
        std::unordered_map<std::string, ComponentInstanceView *> componentInstanceViews_;

    public slots:
        void onComponentAdded(ui::diagram_editor::ComponentViewModel *componentViewModel);
    };
}
