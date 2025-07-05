#include "diagram_service.h"

#include <iostream>
#include <qpoint.h>
#include "id/id_utils.h"

namespace ui::diagram_editor {
    DiagramService::DiagramService(
        std::vector<dcl::Library> *libraries,
        dst::AppSettings *appSettings,
        QObject *parent
    ): QObject(parent),
        libraries_(libraries),

        openedDiagrams_(std::unordered_map<std::string, DiagramViewModel *>()),
        appSettings_(appSettings) {
    }

    DiagramService::~DiagramService() = default;


    void DiagramService::openChart(std::string chartId) {
        if (openedDiagrams_.contains(chartId)) {
            emit chartOpened(chartId);
            return;
        }

        const auto chart = new dd::Diagram(chartId, "Chart_" + std::to_string(openedDiagrams_.size()));

        openedDiagrams_[chartId] = new DiagramViewModel(chart, this);

        emit chartOpened(chartId);
    }


    DiagramViewModel *DiagramService::getDiagram(const std::string &chartId) {
        return openedDiagrams_[chartId];
    }


    void DiagramService::addComponent(
        const std::string &chartId,
        const int libraryId,
        const int componentId,
        const QPointF posi
    ) const {
        if (!openedDiagrams_.contains(chartId)) {
            return;
        }


        DiagramViewModel *diagram = openedDiagrams_.at(chartId);

        std::cout << libraries_->size() << std::endl;

        const auto component = getComponent(libraryId, componentId);

        if (component == nullptr) {
            return;
        }

        auto position = dd::GraphicItemPosition();
        position.x = posi.x();
        position.y = posi.y();

        const std::string name = component->name + "_" + std::to_string(diagram->componentsCount());
        const std::string librariesDir = appSettings_->getComponentsLibraryDir();

        const auto componentInstance = dd::ComponentInstance(
            newId(),
            name,
            component->id,
            position,
            component,
            librariesDir
        );

        diagram->addComponent(componentInstance);
    }

    const dcl::Component *DiagramService::getComponent(const int libraryId, const int componentId) const {
        if (libraryId < 0 || libraryId >= static_cast<int>(libraries_->size())) {
            std::cerr << "Invalid libraryId: " << libraryId << std::endl;
            return nullptr;
        }

        const auto &library = libraries_->at(libraryId);

        if (componentId < 0 || componentId >= static_cast<int>(library.components.size())) {
            std::cerr << "Invalid componentId: " << componentId << std::endl;
            return nullptr;
        }

        return &library.components[componentId];
    }
}
