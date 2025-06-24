#include "chart_interface.h"

#include <qpoint.h>

#include "components_library/model/library.h"
#include "id/id_utils.h"

namespace ui::chart_editor {
    void ChartInterface::addComponent(const int libraryId, const int componentId, const QPointF posi) const {
        auto component = getComponent(libraryId, componentId);
        auto position = dce::GraphicItemPosition();
        position.x = posi.x();
        position.y = posi.y();

        std::string name = component->name + "_001";
        const std::string librariesDir = appSettings_->getComponentsLibraryDir();

        auto componentInstance = std::make_shared<dce::ComponentInstance>(
            newId(), name, component->id,
            position, component,
            librariesDir);

        chart_->addComponent(componentInstance);
        emit componentAdded(componentInstance);
    }

    std::shared_ptr<dcl::Component> ChartInterface::getComponent(const int libraryId, const int componentId) const {
        const auto library = libraries_->at(libraryId);
        const dcl::Component component = library.components[componentId];
        return std::make_shared<dcl::Component>(component);
    }
}
