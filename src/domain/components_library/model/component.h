#pragma once
#include <string>
#include <vector>

#include "component_io.h"

namespace domain::components_library {
    struct Component {
        std::string id;
        std::string name;
        std::string symbolPath;
        std::string fmuPath;
        std::vector<Constant> constants;

        struct {
            std::vector<Port> inputs;
            std::vector<Port> outputs;
        } ports;

        struct {
            std::vector<Signal> inputs;
            std::vector<Signal> outputs;
        } signalsIo;
    };
}
