#pragma once
#include <string>
#include <vector>
#include "component.h"

struct Library {
    std::string id;
    std::string name;
    std::vector<Component> components;
};
