#pragma once
#include <filesystem>
#include "../../../domain/components_library/model/library.h"
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

void to_json(json &j, const ConstantSignal &constantSignal);

void from_json(const json &j, ConstantSignal &constantSignal);

void to_json(json &j, const VariableSignal &variableSignal);

void from_json(const json &j, VariableSignal &variableSignal);

void to_json(json &j, const Component &component);

void from_json(const json &j, Component &component);

void to_json(json &j, const Library &library);

void from_json(const json &j, Library &library);
