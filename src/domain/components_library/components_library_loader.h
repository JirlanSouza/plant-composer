#pragma once
#include <vector>
#include <filesystem>

#include "model/library.h"

namespace fs = std::filesystem;

namespace domain::components_library {
    class ComponentsLibraryLoader {
    public:
        virtual ~ComponentsLibraryLoader() = default;

        virtual std::vector<Library> loadLibraries() = 0;
    };
}
