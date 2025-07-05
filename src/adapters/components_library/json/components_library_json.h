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
#include <nlohmann/json.hpp>

#include "domain/components_library/model/library.h"

using json = nlohmann::json;

namespace domain::components_library {
    void to_json(json &j, const Constant &constant);

    void from_json(const json &j, Constant &constant);

    void to_json(json &j, const Measurement &measurement);

    void from_json(const json &j, Measurement &measurement);

    void to_json(json &j, const PortType &portType);

    void from_json(const json &j, PortType &portType);

    void to_json(json &j, const Port &port);

    void from_json(const json &j, Port &port);

    void to_json(json &j, const Signal &signal);

    void from_json(const json &j, Signal &signal);

    void to_json(json &j, const Component &component);

    void from_json(const json &j, Component &component);

    void to_json(json &j, const Library &library);

    void from_json(const json &j, Library &library);
}
