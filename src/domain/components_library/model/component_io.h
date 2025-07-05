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
