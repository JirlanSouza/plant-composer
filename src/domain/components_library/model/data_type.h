#pragma once
#include <string>

namespace domain::components_library {
    enum class DataType {
        BOOLEAN,
        INTEGER,
        REAL,
        STRING,
    };

    std::string dataTypeToString(const DataType &dataType);

    std::optional<DataType> dataTypeFromString(const std::string &dataTypeStr);
}
