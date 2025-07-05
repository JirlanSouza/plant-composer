#include "diagram_view_model.h"

namespace ui::diagram_editor {
    DiagramViewModel::DiagramViewModel(dd::Diagram *diagram, QObject *parent): QObject(parent), diagram_(diagram) {
        for (auto component: diagram_->getComponents()) {
            const auto newComponentViewModel = new ComponentViewModel(&component, this);
            componentViewModels_.append(newComponentViewModel);
        }
    }

    DiagramViewModel::~DiagramViewModel() = default;

    void DiagramViewModel::addComponent(const dd::ComponentInstance &component) {
        diagram_->addComponent(component);

        const auto newComponentViewModel = new ComponentViewModel(diagram_->getLastAddedComponent(), this);
        componentViewModels_.append(newComponentViewModel);
        emit componentAdded(newComponentViewModel);
    }

    ComponentViewModel *DiagramViewModel::getLastAddedComponentViewModel() {
        if (componentViewModels_.isEmpty()) {
            return nullptr;
        }
        return componentViewModels_.last();
    }

    int DiagramViewModel::componentsCount() const {
        return diagram_->componentsCount();
    }

    std::string DiagramViewModel::getId() const {
        return diagram_->getId();
    }

    std::string DiagramViewModel::getName() const {
        return diagram_->getName();
    }

    int DiagramViewModel::getWidth() const {
        return diagram_->getWidth();
    }

    int DiagramViewModel::getHeight() const {
        return diagram_->getHeight();
    }
}
