#pragma once
#include <QObject>

#include "editor_widget/diagram_view_model.h"
#include "domain/components_library/model/library.h"
#include "domain/settings/app_settings.h"

namespace dd = domain::diagram;
namespace dcl = domain::components_library;
namespace dst = domain::settings;

namespace ui::diagram_editor {
    class DiagramService final : public QObject {
        Q_OBJECT

    public:
        explicit DiagramService(std::vector<dcl::Library> *libraries, dst::AppSettings *appSettings, QObject *parent);

        ~DiagramService() override;

        void openChart(std::string chartId);


        DiagramViewModel *getDiagram(const std::string &chartId);

    signals:
        void chartOpened(std::string chartId);

    private:
        std::vector<dcl::Library> *libraries_;

        std::unordered_map<std::string, DiagramViewModel *> openedDiagrams_;
        dst::AppSettings *appSettings_;

        [[nodiscard]] const dcl::Component *getComponent(int libraryId, int componentId) const;

    public slots:
        void addComponent(const std::string &chartId, int libraryId, int componentId, QPointF posi) const;
    };
}
