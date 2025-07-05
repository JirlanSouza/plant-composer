/*
 * plant-composer
 * Copyright (C) 2025 Jirlan Souza
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

#pragma once
#include "domain/components_library/components_library_loader.h"

using domain::components_library::ComponentsLibraryLoader;
using domain::components_library::Library;

namespace adapters::components_library {
    class JsonComponentsLibraryLoader final : public ComponentsLibraryLoader {
    public:
        explicit JsonComponentsLibraryLoader(const std::string &librariesDir);

        [[nodiscard]] std::vector<Library> loadLibraries() override;

    private:
        const std::string LIB_EXTENSION = ".json";
        fs::path librariesDir_;

        [[nodiscard]] std::vector<fs::path> listLibrariesPaths() const;

        static Library loadLibraryFromPath(fs::path &libraryPath);
    };
}
