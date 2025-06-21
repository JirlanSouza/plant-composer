#pragma once
#include <vector>
#include "model/library.h"

namespace fs = std::filesystem;

class ComponentsLibraryLoader {
public:
    virtual ~ComponentsLibraryLoader() = default;

    virtual std::vector<Library> loadLibraries() = 0;
};
