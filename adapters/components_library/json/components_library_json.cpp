#include "components_library_json.h"
#include <fstream>
#include "../../../domain/components_library/model/signal_data_type.h"

NLOHMANN_JSON_SERIALIZE_ENUM(SignalDataType, {
                             { BOOLEAN, "BOOLEAN"},
                             { INTEGER, "INTEGER"},
                             { REAL, "REAL"}
                             })

void to_json(json &j, const ConstantSignal &constantSignal) {
    j = json{
        {"name", constantSignal.name},
        {"dataType", constantSignal.dataType},
        {"defaultValue", constantSignal.defaultValue}
    };
}

void from_json(const json &j, ConstantSignal &constantSignal) {
    constantSignal.name = j.at("name").get<std::string>();
    constantSignal.dataType = j.at("dataType").get<SignalDataType>();
    constantSignal.defaultValue = j.at("defaultValue").get<std::string>();
}

void to_json(json &j, const VariableSignal &variableSignal) {
    j = json{
        {"name", variableSignal.name},
        {"dataType", variableSignal.dataType},
    };
}

void from_json(const json &j, VariableSignal &variableSignal) {
    variableSignal.name = j.at("name").get<std::string>();
    variableSignal.dataType = j.at("dataType").get<SignalDataType>();
}

void to_json(json &j, const Component &component) {
    j = json{
        {"id", component.id},
        {"name", component.name},
        {"iconPath", component.iconPath},
        {"chartGraphicPath", component.chartGraphicPath},
        {"fmuPath", component.fmuPath},
        {"constants", component.constants},
        {"inputs", component.inputs},
        {"outputs", component.outputs},
    };
}

void from_json(const json &j, Component &component) {
    component.id = j.at("id").get<std::string>();
    component.name = j.at("name").get<std::string>();
    component.iconPath = j.at("iconPath").get<std::string>();
    component.chartGraphicPath = j.at("chartGraphicPath").get<std::string>();
    component.fmuPath = j.at("fmuPath").get<std::string>();
    component.inputs = j.at("inputs").get<std::vector<VariableSignal> >();
    component.constants = j.at("constants").get<std::vector<ConstantSignal> >();
    component.outputs = j.at("outputs").get<std::vector<VariableSignal> >();
}

void to_json(json &j, const Library &library) {
    j = json{
        {"id", library.id},
        {"name", library.name},
        {"components", library.components}
    };
}

void from_json(const json &j, Library &library) {
    library.id = j.at("id").get<std::string>();
    library.name = j.at("name").get<std::string>();
    library.components = j.at("components").get<std::vector<Component> >();
}
