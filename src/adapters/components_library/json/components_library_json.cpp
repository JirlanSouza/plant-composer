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

#include "components_library_json.h"

#include <fstream>

#include "domain/components_library/model/data_type.h"

namespace domain::components_library {
    NLOHMANN_JSON_SERIALIZE_ENUM(
        DataType,
        { { DataType::BOOLEAN,
        "BOOLEAN"},
        { DataType::INTEGER,
        "INTEGER"},
        { DataType::REAL,
        "REAL"},
        { DataType::STRING,
        "STRING"}
        }
    );

    void to_json(json &j, const Constant &constant) {
        j = json{
            {"name", constant.name},
            {"dataType", constant.dataType},
            {"defaultValue", constant.defaultValue}
        };
    }

    void from_json(const json &j, Constant &constant) {
        constant.name = j.at("name").get<std::string>();
        constant.dataType = j.at("dataType").get<DataType>();
        constant.defaultValue = j.at("defaultValue").get<std::string>();
    }

    void to_json(json &j, const Measurement &measurement) {
        j = json{
            {"name", measurement.name},
            {"dataType", measurement.dataType},
            {"unit", measurement.unit},
        };
    }

    void from_json(const json &j, Measurement &measurement) {
        measurement.name = j.at("name").get<std::string>();
        measurement.dataType = j.at("dataType").get<DataType>();
        measurement.unit = j.at("unit").get<std::string>();
    }

    void to_json(json &j, const PortType &portType) {
        j = json{
            {"id", portType.id},
            {"name", portType.name},
            {"measurements", portType.measurements},
        };
    }

    void from_json(const json &j, PortType &portType) {
        portType.id = j.at("id").get<int>();
        portType.name = j.at("name").get<std::string>();
        portType.measurements = j.at("measurements").get<std::vector<Measurement> >();
    }


    void to_json(json &j, const Port &port) {
        j = json{
            {"name", port.name},
            {"typeId", port.typeId}
        };
    }

    void from_json(const json &j, Port &port) {
        port.name = j.at("name").get<std::string>();
        port.typeId = j.at("typeId").get<int>();
    }

    void to_json(json &j, const Signal &signal) {
        j = json{
            {"name", signal.name},
            {"dataType", signal.dataType}
        };
    }

    void from_json(const json &j, Signal &signal) {
        signal.name = j.at("name").get<std::string>();
        signal.dataType = j.at("dataType").get<DataType>();
    }

    void to_json(json &j, const ComponentType &component) {
        json portsJson = {
            {"inputs", component.ports.inputs},
            {"outputs", component.ports.outputs}
        };

        json signalsIoJson = {
            {"inputs", component.signalsIo.inputs},
            {"outputs", component.signalsIo.outputs}
        };

        j = json{
            {"id", component.id},
            {"name", component.name},
            {"symbolPath", component.symbolPath},
            {"fmuPath", component.fmuPath},
            {"constants", component.constants},
            {"ports", portsJson},
            {"signalsIo", signalsIoJson}
        };
    }

    void from_json(const json &j, ComponentType &component) {
        component.id = j.at("id").get<std::string>();
        component.name = j.at("name").get<std::string>();
        component.symbolPath = j.at("symbolPath").get<std::string>();
        component.fmuPath = j.at("fmuPath").get<std::string>();
        component.constants = j.at("constants").get<std::vector<Constant> >();
        component.ports.inputs = j.at("ports").value("inputs", std::vector<Port>{});
        component.ports.outputs = j.at("ports").value("outputs", std::vector<Port>{});
        component.signalsIo.inputs = j.at("signalsIo").value("inputs", std::vector<Signal>{});
        component.signalsIo.outputs = j.at("signalsIo").value("outputs", std::vector<Signal>{});
    }

    void to_json(json &j, const Library &library) {
        j = json{
            {"id", library.id},
            {"name", library.name},
            {"version", library.version},
            {"author", library.author},
            {"description", library.description},
            {"portTypes", library.portTypes},
            {"components", library.components}
        };
    }

    void from_json(const json &j, Library &library) {
        library.id = j.at("id").get<std::string>();
        library.name = j.at("name").get<std::string>();
        library.version = j.at("version").get<std::string>();
        library.author = j.at("author").get<std::string>();
        library.description = j.at("description").get<std::string>();
        library.portTypes = j.at("portTypes").get<std::vector<PortType> >();
        library.components = j.at("components").get<std::vector<ComponentType> >();
    }
}
