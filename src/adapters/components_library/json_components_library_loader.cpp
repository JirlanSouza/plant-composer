#include "json_components_library_loader.h"
#include "json/components_library_json.h"

#include <fstream>

using json = nlohmann::json;

namespace adapters::components_library {
    JsonComponentsLibraryLoader::JsonComponentsLibraryLoader(const std::string &librariesDir)
        : ComponentsLibraryLoader(),
          librariesDir_(fs::path(librariesDir)) {
    }

    std::vector<Library> JsonComponentsLibraryLoader::loadLibraries() {
        std::vector<Library> libraries;
        const std::vector<fs::path> librariesPaths = listLibrariesPaths();

        libraries.reserve(librariesPaths.size());
        for (auto libraryPath: librariesPaths) {
            libraries.push_back(loadLibraryFromPath(libraryPath));
        }
        return libraries;
    }

    std::vector<fs::path> JsonComponentsLibraryLoader::listLibrariesPaths() const {
        std::vector<fs::path> librariesPaths;
        if (!fs::exists(librariesDir_) || !fs::is_directory(librariesDir_)) {
            throw std::runtime_error("Invalid libraries directory: " + librariesDir_.string());
        }

        for (const fs::directory_entry &entry: fs::directory_iterator(librariesDir_)) {
            if (entry.is_regular_file() && entry.path().extension() == LIB_EXTENSION) {
                librariesPaths.push_back(entry.path());
            }
        }

        return librariesPaths;
    }

    Library JsonComponentsLibraryLoader::loadLibraryFromPath(fs::path &libraryPath) {
        std::ifstream inputStream(libraryPath);
        const json libraryJson = json::parse(inputStream);
        Library library;
        from_json(libraryJson, library);
        return library;
    }
}
