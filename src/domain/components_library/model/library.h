#pragma once
#include <string>
#include <vector>

#include "component.h"

namespace domain::components_library {
    struct Library {
        std::string id;
        std::string name;
        std::string version;
        std::string author;
        std::vector<PortType> portTypes;
        std::vector<Component> components;
    };
}
