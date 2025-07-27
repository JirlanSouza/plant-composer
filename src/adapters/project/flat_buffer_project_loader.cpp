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

#include "flat_buffer_project_loader.h"

#include <fstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace project {
    FlatBufferProjectLoader::FlatBufferProjectLoader(
        common::ILoggerFactory *loggerFactory,
        common::IDFactory *idFactory
    ): logger_(loggerFactory->getLogger("FlatBufferProjectLoader")), idFactory_(idFactory) {
        parser_ = std::make_unique<FlatBufferProjectParser>(loggerFactory);
        if (!logger_) {
            throw std::runtime_error("Failed to create logger for FlatBufferProjectLoader");
        }
        logger_->info("FlatBufferProjectLoader initialized successfully");
    }

    std::optional<std::unique_ptr<Project> > FlatBufferProjectLoader::createNewProject(
        const std::string &name,
        const std::string &description,
        const std::string &author,
        const std::string &parentDirectory
    ) {
        logger_->info(
            "Creating new project with name: {}, description: {}, author: {}, parent directory: {}",
            name,
            description,
            author,
            parentDirectory
        );
        const fs::path projectPath = fs::path(parentDirectory) / name;
        if (!fs::exists(parentDirectory) || !fs::is_directory(parentDirectory) || fs::exists(projectPath)) {
            logger_->warn("Invalid project path: {}", projectPath.string());
            return std::nullopt;
        }

        fs::create_directory(projectPath);
        fs::create_directory(projectPath / "diagrams");

        auto project = std::make_unique<Project>(
            idFactory_->create(),
            name,
            description,
            author,
            (projectPath / (name + ".pcp")).string()
        );

        saveProject(*project);
        logger_->info("Project created successfully at: {}", projectPath.string());
        return project;
    }

    std::optional<std::unique_ptr<Project> > FlatBufferProjectLoader::loadProject(const std::string &path) {
        try {
            logger_->info("Loading project from path: {}", path);
            std::ifstream infile(path, std::ios::binary);

            if (!infile) {
                logger_->warn("Failed to open project file: {}", path);
                return std::nullopt;
            }

            const std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
            if (buffer.empty()) {
                logger_->warn("Failed to read project file: {} has empty data", path);
                infile.close();
                return std::nullopt;
            }

            logger_->info("Loaded project file: {}", path);
            return parser_->parse(buffer, path);
        } catch (const std::exception &e) {
            logger_->error("Error loading project from path: {}. Exception: {}", path, e.what());
            return std::nullopt;
        }
    }

    void FlatBufferProjectLoader::saveProject(const Project &project) {
        logger_->info("Saving project with name: {} to path: {}", project.getName(), project.getPath());
        auto buffer = parser_->serialize(project);
        std::ofstream outfile(project.getPath(), std::ios::binary);
        outfile.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
        outfile.close();
        logger_->info("Project saved successfully to: {}", project.getPath());
    }

    std::optional<std::unique_ptr<diagram::Diagram> > FlatBufferProjectLoader::loadDiagram(
        const DiagramMetadata &metadata
    ) {
        // TODO: Implement diagram loading using parser
        return std::nullopt;
    }

    void FlatBufferProjectLoader::saveDiagram(const DiagramMetadata &metadata, const diagram::Diagram &diagram) {
        // TODO: Implement diagram saving using parser
    }
}
