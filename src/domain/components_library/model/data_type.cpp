/*
 * plant-composer
 * Copyright (c) 2025 jirlansouza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
