/*
 * plant-composer
 * Copyright (c) 2025 jirlansouza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "json_components_library_loader.h"

#include <fstream>

#include "json/components_library_json.h"


using json = nlohmann::json;

namespace components_library {
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
