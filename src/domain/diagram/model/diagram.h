#pragma once
#include "component_instance.h"

namespace domain::diagram {
    class Diagram {
    public:
        Diagram(const std::string &id, const std::string &name);

        ~Diagram();

        void addComponent(const ComponentInstance &component);

        ComponentInstance *getLastAddedComponent();

        [[nodiscard]] int componentsCount() const;

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] std::vector<ComponentInstance> getComponents() const;

    private:
        std::string id_;
        std::string name_;
        int width_;
        int height_;
        std::vector<ComponentInstance> components_;
    };
}
