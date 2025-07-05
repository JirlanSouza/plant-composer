#pragma once
#include <string>

#include "data_type.h"

namespace domain::components_library {
    struct Constant {
        std::string name;
        DataType dataType;
        std::string defaultValue;
    };

    struct Measurement {
        std::string name;
        DataType dataType;
        std::string unit;
    };

    struct PortType {
        int id;
        std::string name;
        std::vector<Measurement> measurements;
    };

    struct Port {
        std::string name;
        int typeId;
    };

    struct Signal {
        std::string name;
        DataType dataType;
    };
}
