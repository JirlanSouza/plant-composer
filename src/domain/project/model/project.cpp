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

#include "project.h"

namespace domain::project {
    Project::Project(
        const std::string &id,
        const std::string &name,
        const std::string &description,
        const std::string &author,
        const std::string &version,
        const std::string &path,
        std::unique_ptr<ProjectCategory<DiagramMetadata> > diagrams
    )
        : id_(id),
        name_(name),
        description_(description),
        author_(author),
        version_(version),
        path_(path) {
        diagrams_ = std::move(diagrams);
        // scripts_ = std::make_unique<ProjectCategory<ScriptMetadata>>("scripts_root", "Scripts");
        // drivers_ = std::make_unique<ProjectCategory<DriverMetadata>>("drivers_root", "Drivers");
    }

    Project::~Project() = default;

    std::string Project::getId() const { return id_; }
    std::string Project::getName() const { return name_; }
    std::string Project::getDescription() const { return description_; }
    std::string Project::getAuthor() const { return author_; }
    std::string Project::getVersion() const { return version_; }
    std::string Project::getPath() const { return path_; }

    ProjectCategory<DiagramMetadata> *Project::diagrams() const {
        return diagrams_.get();
    }

    DiagramMetadata::DiagramMetadata(
        const std::string &id,
        NodeContainer<DiagramMetadata> *parent,
        const std::string &name,
        const std::string &filePath
    ): FileNode<DiagramMetadata>(id, parent, name, filePath) {
    }
}
