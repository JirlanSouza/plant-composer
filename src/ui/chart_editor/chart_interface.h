#pragma once
#include <QObject>

#include "chart_editor/model/chart.h"
#include "settings/app_settings.h"

namespace domain::components_library {
    struct Library;
}

namespace dce = domain::chart_editor;
namespace dcl = domain::components_library;
namespace dst = domain::settings;

namespace ui::chart_editor {
    class ChartInterface final : public QObject {
        Q_OBJECT

    public:
        explicit ChartInterface(QObject *parent,
                                const std::shared_ptr<std::vector<dcl::Library> > &libraries,
                                const std::shared_ptr<dce::Chart> &chart,
                                const std::shared_ptr<dst::AppSettings> &appSettings): QObject(parent),
            libraries_(libraries),
            chart_(chart), appSettings_(appSettings) {
        };

        ~ChartInterface() override = default;

    signals:
        void componentAdded(std::shared_ptr<dce::ComponentInstance> &componentInstance) const;

    private:
        std::shared_ptr<std::vector<dcl::Library> > libraries_;
        std::shared_ptr<dce::Chart> chart_;
        std::shared_ptr<dst::AppSettings> appSettings_;

        std::shared_ptr<dcl::Component> getComponent(int libraryId, int componentId) const;

    public slots:
        void addComponent(int libraryId, int componentId, QPointF posi) const;
    };
}
