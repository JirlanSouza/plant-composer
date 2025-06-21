#pragma once
#include <string>
#include <vector>
#include "constant_signal.h"
#include "variable_signal.h"

struct Component {
    std::string id;
    std::string name;
    std::string iconPath;
    std::string chartGraphicPath;
    std::string fmuPath;
    std::vector<ConstantSignal> constants;
    std::vector<VariableSignal> inputs;
    std::vector<VariableSignal> outputs;
};
