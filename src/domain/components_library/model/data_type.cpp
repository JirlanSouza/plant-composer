#include "data_type.h"

namespace domain::components_library {
    std::string dataTypeToString(const DataType &dataType) {
        switch (dataType) {
            case DataType::BOOLEAN: return "BOOLEAN";
            case DataType::INTEGER: return "INTEGER";
            case DataType::REAL: return "REAL";
            case DataType::STRING: return "STRING";
            default: return "UNKNOWN";
        }
    }

    std::optional<DataType> dataTypeFromString(const std::string &dataTypeStr) {
        if (dataTypeStr == "BOOLEAN") return DataType::BOOLEAN;
        if (dataTypeStr == "INTEGER") return DataType::INTEGER;
        if (dataTypeStr == "REAL") return DataType::REAL;
        if (dataTypeStr == "STRING") return DataType::STRING;
        return std::nullopt;
    }
}
