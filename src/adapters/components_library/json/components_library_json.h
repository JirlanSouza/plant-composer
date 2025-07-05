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
