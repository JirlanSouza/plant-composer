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

#include "flat_buffer_project_parser.h"

#include <iostream>

#include "adapters/serialization/flatbuffers/project_generated.h"

namespace adapters::project {
    FlatBufferProjectParser::FlatBufferProjectParser(common::ILoggerFactory *loggerFactory): logger_(
        loggerFactory->getLogger("FlatBufferProjectParser")
    ) {
        if (!logger_) {
            throw std::runtime_error("Failed to create logger for FlatBufferProjectParser");
        }
        logger_->info("FlatBufferProjectParser initialized successfully");
    }

    FbProjectNode FlatBufferProjectParser::serializeProjectNode(
        flatbuffers::FlatBufferBuilder &builder,
        dp::ProjectNode *domainNode
    ) {
        FbProjectNode fbProjectNode{0, plant_composer::fbs::ProjectNode_NONE};

        if (domainNode->isFile()) {
            const auto *fileNode = domainNode->getAsFile().value();
            logger_->info(
                "Serializing project FileNode with ID: {}, name: {}",
                fileNode->getId(),
                fileNode->getName()
            );

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
            logger_->info(
                "Serializing project FolderNode with ID: {}, name: {}, children: {}",
                folderNode->getId(),
                folderNode->getName(),
                folderNode->getChildren().size()
            );

            std::vector<flatbuffers::Offset<void> > fbChildren;
            std::vector<plant_composer::fbs::ProjectNode> fbTypes;

            for (dp::ProjectNode *child: folderNode->getChildren()) {
                auto [nodeOffset, nodeType] = serializeProjectNode(builder, child);
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

        logger_->info(
            "Successfully serialized project node with ID: {}, name: {}",
            domainNode->getId(),
            domainNode->getName()
        );
        return fbProjectNode;
    }

    void FlatBufferProjectParser::parserFlatBufferProjectNodes(
        const flatbuffers::Vector<flatbuffers::Offset<void> > *nodes,
        const flatbuffers::Vector<u_int8_t> *types,
        dp::NodeContainer *parent
    ) {
        if (!parent) {
            logger_->error("Received null parent");
            throw std::runtime_error("Received null parent");
        }

        std::vector<dp::ProjectNode> projectNodes;

        if (!nodes || !types || nodes->size() != types->size()) {
            logger_->warn("Invalid FlatBuffer project nodes or types provided");
            return;
        }

        for (size_t i = 0; i < nodes->size(); ++i) {
            const auto fbType = types->Get(i);
            const auto fbNode = nodes->Get(i);

            if (fbType == plant_composer::fbs::ProjectNode_FileNode) {
                const auto file = static_cast<const plant_composer::fbs::FileNode *>(fbNode);
                logger_->info(
                    "Parsing FlatBuffer project FileNode with ID: {}, name: {}",
                    file->id()->str(),
                    file->name()->str()
                );
                std::unique_ptr<dp::ProjectNode> domainFileNode = std::make_unique<dp::FileNode>(
                    file->id()->str(),
                    parent,
                    file->name()->str(),
                    file->file_path()->str()
                );
                parent->addChild(std::move(domainFileNode));
            } else if (fbType == plant_composer::fbs::ProjectNode_FolderNode) {
                const auto folder = static_cast<const plant_composer::fbs::FolderNode *>(fbNode);
                logger_->info(
                    "Parsing FlatBuffer project FolderNode with ID: {}, name: {}, children: {}",
                    folder->id()->str(),
                    folder->name()->str(),
                    folder->children()->size()
                );
                std::unique_ptr<dp::ProjectNode> domainFolder = std::make_unique<dp::NodeContainer>(
                    folder->id()->str(),
                    parent,
                    folder->name()->str()
                );

                parserFlatBufferProjectNodes(
                    folder->children(),
                    folder->children_type(),
                    dynamic_cast<dp::NodeContainer *>(domainFolder.get())
                );
                parent->addChild(std::move(domainFolder));
            }
        }

        logger_->info("Successfully parsed FlatBuffer project nodes with total: {} nodes", nodes->size());
    }

    flatbuffers::Offset<plant_composer::fbs::ProjectCategory> FlatBufferProjectParser::serializeProjectCategory(
        flatbuffers::FlatBufferBuilder &builder,
        const dp::ProjectCategory *category
    ) {
        logger_->info(
            "Serializing FlatBuffer project category with ID: {}, name: {}, children count: {}",
            category->getId(),
            category->getName(),
            category->getChildren().size()
        );
        std::vector<flatbuffers::Offset<void> > fbChildren;
        std::vector<plant_composer::fbs::ProjectNode> fbTypes;

        for (const auto &child: category->getChildren()) {
            if (child->isFile()) {
                logger_->info(
                    "Serialize FileNode with ID: {}, name: {}",
                    child->getId(),
                    child->getName()
                );
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
                logger_->info("Serialize FolderNode with ID: {}, name: {}", child->getId(), child->getName());
                auto [nodeOffset, nodeType] = serializeProjectNode(builder, child->getAsFolder().value());
                fbChildren.push_back(nodeOffset);
                fbTypes.push_back(nodeType);
            }
        }

        const auto fbChildrenVector = builder.CreateVector(fbChildren);
        const auto fbTypesVector = builder.CreateVector(
            reinterpret_cast<const uint8_t *>(fbTypes.data()),
            fbTypes.size()
        );

        logger_->info(
            "Successfully serialized FlatBuffer project category with ID: {}, name: {}",
            category->getId(),
            category->getName()
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

    std::unique_ptr<dp::ProjectCategory> FlatBufferProjectParser::parserFlatBufferProjectCategory(
        const plant_composer::fbs::ProjectCategory *fbProjectCategory
    ) {
        logger_->info(
            "Parsing project category from FlatBuffer with ID: {}, name: {}, children count: {}",
            fbProjectCategory->id()->str(),
            fbProjectCategory->name()->str(),
            fbProjectCategory->children()->size()
        );

        auto category = std::make_unique<dp::ProjectCategory>(
            fbProjectCategory->id()->str(),
            fbProjectCategory->name()->str(),
            fbProjectCategory->folder_name()->str()
        );

        for (size_t i = 0; i < fbProjectCategory->children()->size(); ++i) {
            parserFlatBufferProjectNodes(
                fbProjectCategory->children(),
                fbProjectCategory->children_type(),
                category.get()
            );
        }

        logger_->info(
            "Successfully parsed project diagrams from FlatBuffer with total: {} nodes",
            category->getChildren().size()
        );
        return category;
    }


    std::optional<std::unique_ptr<dp::Project> > FlatBufferProjectParser::parse(
        const std::vector<char> &buffer,
        const std::string &path
    ) {
        if (buffer.empty() || !plant_composer::fbs::SizePrefixedProjectBufferHasIdentifier(buffer.data())) {
            logger_->warn("FlatBufferProjectParser: invalid buffer provided");
            return std::nullopt;
        }

        auto *projectTable = flatbuffers::GetSizePrefixedRoot<plant_composer::fbs::Project>(buffer.data());

        auto project = std::make_unique<dp::Project>(
            projectTable->id()->str(),
            projectTable->name()->str(),
            projectTable->description()->str(),
            projectTable->author()->str(),
            projectTable->version()->str(),
            path,
            parserFlatBufferProjectCategory(projectTable->diagrams())
        );

        if (!project->diagrams()) {
            logger_->warn("FlatBufferProjectParser: failed to parse diagrams from project");
            return std::nullopt;
        }

        logger_->info("FlatBufferProjectParser: successfully parsed project");
        return project;
    }

    flatbuffers::DetachedBuffer FlatBufferProjectParser::serialize(const dp::Project &project) {
        flatbuffers::FlatBufferBuilder builder;

        const auto id = builder.CreateString(project.getId());
        const auto name = builder.CreateString(project.getName());
        const auto description = builder.CreateString(project.getDescription());
        const auto author = builder.CreateString(project.getAuthor());
        const auto version = builder.CreateString(project.getVersion());

        const auto fbDiagramsRoot = serializeProjectCategory(builder, project.diagrams());

        const auto projectTable = plant_composer::fbs::CreateProject(
            builder,
            id,
            name,
            description,
            author,
            version,
            fbDiagramsRoot
        );

        plant_composer::fbs::FinishSizePrefixedProjectBuffer(builder, projectTable);
        return builder.Release();
    }
}
