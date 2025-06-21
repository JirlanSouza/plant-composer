#include "signal_data_type.h"

std::string to_string(const SignalDataType &dataType) {
    switch (dataType) {
        case BOOLEAN: return "BOOLEAN";
        case INTEGER: return "INTEGER";
        case REAL: return "REAL";
    }
    return "INVALID";
}
