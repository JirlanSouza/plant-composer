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

#pragma once

#include <memory>
#include <string>
#include <optional>

#include "model/project.h"
#include "domain/diagram/model/diagram.h"

namespace project {
    class IProjectLoader {
    public:
        virtual ~IProjectLoader() = default;

        virtual std::optional<std::unique_ptr<Project> > createNewProject(
            const std::string &name,
            const std::string &description,
            const std::string &author,
            const std::string &parentDirectory
        ) = 0;

        virtual std::optional<std::unique_ptr<Project> > loadProject(const std::string &path) = 0;

        virtual void saveProject(const Project &project) = 0;

        virtual std::optional<std::unique_ptr<diagram::Diagram> > loadDiagram(const FileNode &metadata) = 0;

        virtual void saveDiagram(const FileNode &metadata, const diagram::Diagram &diagram) = 0;
    };
}
