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

#include "adapters/serialization/flatbuffers/project_generated.h"

namespace adapters::project {
    std::unique_ptr<dp::Project> FlatBufferProjectLoader::loadProject(const std::string &path) {
        std::ifstream infile(path, std::ios::binary);
        if (!infile) {
            return nullptr;
        }

        std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
        auto *projectTable = flatbuffers::GetRoot<plant_composer::fbs::ProjectTable>(buffer.data());

        auto project = std::make_unique<dp::Project>(
            projectTable->id()->str(),
            projectTable->name()->str(),
            projectTable->description()->str(),
            projectTable->author()->str(),
            projectTable->version()->str(),
            path
        );

        for (const auto *diagramMetadata: *projectTable->diagrams_metadata()) {
            project->addDiagramMetadata(
                {
                    diagramMetadata->id()->str(),
                    diagramMetadata->name()->str(),
                    diagramMetadata->file_path()->str()
                }
            );
        }

        return project;
    }

    void FlatBufferProjectLoader::saveProject(const dp::Project &project) {
        flatbuffers::FlatBufferBuilder builder;

        auto id = builder.CreateString(project.getId());
        auto name = builder.CreateString(project.getName());
        auto description = builder.CreateString(project.getDescription());
        auto author = builder.CreateString(project.getAuthor());
        auto version = builder.CreateString(project.getVersion());

        std::vector<flatbuffers::Offset<plant_composer::fbs::DiagramMetadata> > diagramsMetadataVector;
        for (const auto &metadata: project.getDiagramsMetadata()) {
            auto diagramId = builder.CreateString(metadata.id);
            auto diagramName = builder.CreateString(metadata.name);
            auto diagramFilePath = builder.CreateString(metadata.filePath);
            diagramsMetadataVector.push_back(
                plant_composer::fbs::CreateDiagramMetadata(builder, diagramId, diagramName, diagramFilePath)
            );
        }

        auto diagramsMetadata = builder.CreateVector(diagramsMetadataVector);

        auto projectTable = plant_composer::fbs::CreateProjectTable(
            builder,
            id,
            name,
            description,
            author,
            version,
            diagramsMetadata
        );
        builder.Finish(projectTable);

        std::ofstream outfile(project.getPath(), std::ios::binary);
        outfile.write(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize());
    }

    std::unique_ptr<dd::Diagram> FlatBufferProjectLoader::loadDiagram(
        const dp::Project::DiagramMetadata &metadata
    ) {
        // TODO: Implement diagram loading
        return nullptr;
    }

    void FlatBufferProjectLoader::saveDiagram(const dd::Diagram &diagram) {
        // TODO: Implement diagram saving
    }
}
