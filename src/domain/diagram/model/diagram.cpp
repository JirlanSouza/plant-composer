#include "diagram.h"

namespace domain::diagram {
    Diagram::Diagram(const std::string &id, const std::string &name): id_(id), name_(name), width_(1920), height_(1080),
        components_(std::vector<ComponentInstance>()) {
    };

    Diagram::~Diagram() = default;


    void Diagram::addComponent(const ComponentInstance &component) {
        components_.emplace_back(component);
    }

    ComponentInstance *Diagram::getLastAddedComponent() {
        return &components_[components_.size() - 1];
    }


    int Diagram::componentsCount() const {
        return static_cast<int>(components_.size());
    }

    std::string Diagram::getId() const {
        return id_;
    }

    std::string Diagram::getName() const {
        return name_;
    }


    int Diagram::getWidth() const {
        return width_;
    }

    int Diagram::getHeight() const {
        return height_;
    }

    std::vector<ComponentInstance> Diagram::getComponents() const {
        return components_;
    }
}
