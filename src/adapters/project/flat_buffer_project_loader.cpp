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
#include "adapters/serialization/flatbuffers/diagram_generated.h"

namespace adapters::project {
    struct FbProjectNode {
        flatbuffers::Offset<void> nodeOffset;
        plant_composer::fbs::ProjectNode type;
    };

    FbProjectNode CreateFlatBufferProjectNode(
        flatbuffers::FlatBufferBuilder &builder,
        dp::ProjectNode *domainNode
    ) {
        FbProjectNode fbProjectNode{0, plant_composer::fbs::ProjectNode_NONE};

        if (domainNode->isFile()) {
            const auto *fileNode = domainNode->getAsFile().value();

            const auto fbFileNode = plant_composer::fbs::CreateFileNode(
                builder,
                builder.CreateString(fileNode->getId()),
                builder.CreateString(fileNode->getName()),
                builder.CreateString(fileNode->getFilePath())
            );

            fbProjectNode.nodeOffset = fbFileNode.Union();
            fbProjectNode.type = plant_composer::fbs::ProjectNode_FileNode;
        } else if (domainNode->isFolder()) {
            const auto *folderNode = domainNode->getAsFolder().value();

            std::vector<flatbuffers::Offset<void> > fbChildren;
            std::vector<plant_composer::fbs::ProjectNode> fbTypes;

            for (dp::ProjectNode *child: folderNode->getChildren()) {
                auto [nodeOffset, nodeType] = CreateFlatBufferProjectNode(builder, child);
                fbChildren.push_back(nodeOffset);
                fbTypes.push_back(nodeType);
            }

            const auto fbChildrenVector = builder.CreateVector(fbChildren);
            const auto fbTypesVector = builder.CreateVector(
                reinterpret_cast<const uint8_t *>(fbTypes.data()),
                fbTypes.size()
            );

            const auto fbFolder = plant_composer::fbs::CreateFolderNode(
                builder,
                builder.CreateString(folderNode->getId()),
                folderNode->getParent()
                    ? builder.CreateString(folderNode->getParent()->getId())
                    : 0,
                builder.CreateString(folderNode->getName()),
                fbTypesVector,
                fbChildrenVector
            );

            fbProjectNode.nodeOffset = fbFolder.Union();
            fbProjectNode.type = plant_composer::fbs::ProjectNode_FolderNode;
        }

        return fbProjectNode;
    }

    void ParserFlatBufferProjectNodes(
        const flatbuffers::Vector<flatbuffers::Offset<void> > *nodes,
        const flatbuffers::Vector<u_int8_t> *types,
        dp::NodeContainer *parent
    ) {
        std::vector<dp::ProjectNode> projectNodes;

        if (!nodes || !types || nodes->size() != types->size()) {
            return;
        }

        for (size_t i = 0; i < nodes->size(); ++i) {
            const auto fbType = types->Get(i);
            const auto fbNode = nodes->Get(i);

            if (fbType == plant_composer::fbs::ProjectNode_FileNode) {
                const auto file = static_cast<const plant_composer::fbs::FileNode *>(fbNode);
                std::unique_ptr<dp::ProjectNode> domainFileNode = std::make_unique<dp::FileNode>(
                    file->id()->str(),
                    parent,
                    file->name()->str(),
                    file->file_path()->str()
                );
                parent->addChild(std::move(domainFileNode));
            } else if (fbType == plant_composer::fbs::ProjectNode_FolderNode) {
                const auto folder = static_cast<const plant_composer::fbs::FolderNode *>(fbNode);
                std::unique_ptr<dp::ProjectNode> domainFolder = std::make_unique<dp::NodeContainer>(
                    folder->id()->str(),
                    parent,
                    folder->name()->str()
                );
                parent->addChild(std::move(domainFolder));
                ParserFlatBufferProjectNodes(
                    folder->children(),
                    folder->children_type(),
                    dynamic_cast<dp::NodeContainer *>(domainFolder.get())
                );
            }
        }
    }

    flatbuffers::Offset<plant_composer::fbs::ProjectCategory> CreateFlatBufferProjectCategory(
        flatbuffers::FlatBufferBuilder &builder,
        const dp::ProjectCategory *category
    ) {
        std::vector<flatbuffers::Offset<void> > fbChildren;
        std::vector<plant_composer::fbs::ProjectNode> fbTypes;

        for (const auto &child: category->getChildren()) {
            if (child->isFile()) {
                const auto *fileNode = child->getAsFile().value();
                const auto fbFileNode = plant_composer::fbs::CreateFileNode(
                    builder,
                    builder.CreateString(fileNode->getId()),
                    builder.CreateString(fileNode->getName()),
                    builder.CreateString(fileNode->getFilePath())
                );
                fbChildren.push_back(fbFileNode.Union());
                fbTypes.push_back(plant_composer::fbs::ProjectNode_FileNode);
            } else if (child->isFolder()) {
                auto [nodeOffset, nodeType] = CreateFlatBufferProjectNode(builder, child->getAsFolder().value());
                fbChildren.push_back(nodeOffset);
                fbTypes.push_back(nodeType);
            }

            const auto fbProjectCategory = plant_composer::fbs::CreateProjectCategory(
                builder,
                builder.CreateString(category->getId()),
                builder.CreateString(category->getName()),
                builder.CreateString(category->getFolderName()),
                builder.CreateVector(
                    reinterpret_cast<const uint8_t *>(fbTypes.data()),
                    fbTypes.size()
                ),
                builder.CreateVector(fbChildren)
            );

            return fbProjectCategory;
        }

        const auto fbChildrenVector = builder.CreateVector(fbChildren);
        const auto fbTypesVector = builder.CreateVector(
            reinterpret_cast<const uint8_t *>(fbTypes.data()),
            fbTypes.size()
        );

        return plant_composer::fbs::CreateProjectCategory(
            builder,
            builder.CreateString(category->getId()),
            builder.CreateString(category->getName()),
            builder.CreateString(category->getFolderName()),
            fbTypesVector,
            fbChildrenVector
        );
    }

    std::unique_ptr<dp::ProjectCategory> ParserFlatBufferProjectCategory(
        const plant_composer::fbs::ProjectCategory *fbProjectCategory
    ) {
        auto category = std::make_unique<dp::ProjectCategory>(
            fbProjectCategory->id()->str(),
            fbProjectCategory->name()->str(),
            fbProjectCategory->folder_name()->str()
        );

        for (size_t i = 0; i < fbProjectCategory->children()->size(); ++i) {
            ParserFlatBufferProjectNodes(
                fbProjectCategory->children(),
                fbProjectCategory->children_type(),
                category.get()
            );
        }

        return category;
    }


    std::optional<std::unique_ptr<dp::Project> > FlatBufferProjectLoader::loadProject(const std::string &path) {
        std::ifstream infile(path, std::ios::binary);
        if (!infile) {
            return std::nullopt;
        }
        const std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

        if (buffer.empty() || !plant_composer::fbs::ProjectBufferHasIdentifier(buffer.data())) {
            infile.close();
            return std::nullopt;
        }

        auto *projectTable = flatbuffers::GetRoot<plant_composer::fbs::Project>(buffer.data());

        auto project = std::make_unique<dp::Project>(
            projectTable->id()->str(),
            projectTable->name()->str(),
            projectTable->description()->str(),
            projectTable->author()->str(),
            projectTable->version()->str(),
            path,
            ParserFlatBufferProjectCategory(projectTable->diagrams())
        );
        return project;
    }

    void FlatBufferProjectLoader::saveProject(const dp::Project &project) {
        flatbuffers::FlatBufferBuilder builder;

        const auto id = builder.CreateString(project.getId());
        const auto name = builder.CreateString(project.getName());
        const auto description = builder.CreateString(project.getDescription());
        const auto author = builder.CreateString(project.getAuthor());
        const auto version = builder.CreateString(project.getVersion());

        const auto fbDiagramsRoot = CreateFlatBufferProjectCategory(builder, project.diagrams());

        const auto projectTable = plant_composer::fbs::CreateProject(
            builder,
            id,
            name,
            description,
            author,
            version,
            fbDiagramsRoot
        );
        builder.Finish(projectTable);

        std::ofstream outfile(project.getPath(), std::ios::binary);
        outfile.write(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize());
    }

    std::optional<std::unique_ptr<dd::Diagram> > FlatBufferProjectLoader::loadDiagram(
        const dp::DiagramMetadata &metadata
    ) {
        std::ifstream infile(metadata.getFilePath(), std::ios::binary);
        if (!infile) {
            return nullptr;
        }

        const std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
        auto *diagramTable = flatbuffers::GetRoot<plant_composer::fbs::DiagramTable>(buffer.data());

        auto diagram = std::make_unique<dd::Diagram>(
            diagramTable->id()->str(),
            diagramTable->name()->str()
        );

        // TODO: Load components and connections

        return diagram;
    }

    void FlatBufferProjectLoader::saveDiagram(const dp::DiagramMetadata &metadata, const dd::Diagram &diagram) {
        flatbuffers::FlatBufferBuilder builder;

        auto id = builder.CreateString(diagram.getId());
        auto name = builder.CreateString(diagram.getName());

        // TODO: Save components and connections

        auto diagramTable = plant_composer::fbs::CreateDiagramTable(
            builder,
            id,
            name,
            0,
            // width
            0,
            // height
            0,
            // components
            0 // connections
        );
        builder.Finish(diagramTable);

        std::ofstream outfile(metadata.getFilePath(), std::ios::binary);
        outfile.write(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize());
    }
}
