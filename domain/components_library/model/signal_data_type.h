#pragma once
#include <nlohmann/json.hpp>

enum SignalDataType {
    BOOLEAN,
    INTEGER,
    REAL
};

std::string to_string(const SignalDataType &dataType);
