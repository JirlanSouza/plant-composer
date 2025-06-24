#include "chart.h"

namespace domain::chart_editor {
    void Chart::addComponent(std::shared_ptr<ComponentInstance> component) {
        components_.emplace_back(std::move(component));
    }
}
