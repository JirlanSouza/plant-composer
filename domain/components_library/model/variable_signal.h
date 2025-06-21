#pragma once
#include <string>
#include "signal_data_type.h"

struct VariableSignal {
    std::string name;
    SignalDataType dataType;
    std::string defaultValue;
};
