#pragma once
#include <string>
#include "signal_data_type.h"

struct ConstantSignal {
    std::string name;
    SignalDataType dataType;
    std::string defaultValue;
};
