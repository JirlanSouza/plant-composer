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
