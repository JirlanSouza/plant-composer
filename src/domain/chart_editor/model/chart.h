#pragma once
#include "component_instance.h"

namespace domain::chart_editor {
    class Chart {
    public:
        explicit Chart(const std::string &id, const std::string &name): id_(id), name_(name) {
        };

        ~Chart() = default;

        void addComponent(std::shared_ptr<ComponentInstance> component);

    private:
        std::string id_;
        std::string name_;
        std::vector<std::shared_ptr<ComponentInstance> > components_;
    };
}
