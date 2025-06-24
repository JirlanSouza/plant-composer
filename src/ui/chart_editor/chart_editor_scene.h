#pragma once
#include <QGraphicsScene>
#include <QPainter>

#include "chart_editor/model/component_instance.h"
#include "component_instance_view.h"

namespace ui::chart_editor {
    class ChartEditorScene final : public QGraphicsScene {
        Q_OBJECT

    public:
        explicit ChartEditorScene(QObject *parent = nullptr);

        ~ChartEditorScene() override = default;

    protected:
        void drawBackground(QPainter *painter, const QRectF &rect) override;

    private:
        QObject *parent_;
        const int gridSize = 50;
        const QColor gridColor = QColor(220, 220, 220);
        std::unordered_map<std::string, std::shared_ptr<dce::ComponentInstance> > componentInstances_;
        std::unordered_map<std::string, ComponentInstanceView *> componentInstanceViews_;

    public slots:
        void onComponentAdded(const std::shared_ptr<dce::ComponentInstance>& componentInstance);
    };
}
