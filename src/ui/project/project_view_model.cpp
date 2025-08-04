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

#include "project_view_model.h"

#include <iomanip>

#include "adapters/serialization/flatbuffers/project_generated.h"
#include "domain/common/ilogger_factory.h"
#include "domain/project/project_loader.h"

namespace project {
    ProjectViewModel::ProjectViewModel(
        common::ILoggerFactory *loggerFactory,
        common::IDFactory *idFactory,
        IProjectLoader *projectLoader,
        QObject *parent
    )
        : QObject(parent), logger_(loggerFactory->getLogger("ProjectViewModel")), idFactory_(idFactory),
        projectLoader_(projectLoader) {
    }

    Project *ProjectViewModel::getProject() const {
        return project_.get();
    }

    bool ProjectViewModel::hasOpenedProject() const {
        return project_ != nullptr;
    }

    void ProjectViewModel::createNewProject(
        const std::string &name,
        const std::string &description,
        const std::string &author,
        const std::string &parentDirectory
    ) {
        logger_->info("Attempting to create new project: {}", name);
        auto projectOpt = projectLoader_->createNewProject(name, description, author, parentDirectory);
        if (!projectOpt.has_value()) {
            logger_->error("Failed to create project: {} on directory: {}", name, parentDirectory);
            emit projectCreateFailed(parentDirectory);
            return;
        }

        project_ = std::move(projectOpt.value());
        emit projectOpened();
        logger_->info("Project '{}' created and opened successfully.", name);
    }

    void ProjectViewModel::openProject(const std::string &path) {
        logger_->info("Attempting to open project from path: {}", path);
        if (hasOpenedProject()) {
            closeProject();
        }

        auto projectOpt = projectLoader_->loadProject(path);

        if (!projectOpt.has_value()) {
            logger_->warn("Error loading project from path: {}", path);
            emit projectOpenFailed(path);
            return;
        }

        project_ = std::move(projectOpt.value());
        logger_->info("Project '{}' opened successfully from path: {}", project_->getName(), path);
        emit projectOpened();
    }

    void ProjectViewModel::saveProject() const {
        logger_->info("Attempting to save project: {}", project_->getName());
        if (!project_) {
            logger_->warn("Save project requested but no project is open.");
            return;
        }

        projectLoader_->saveProject(*project_);
        logger_->info("Project '{}' saved successfully.", project_->getName());
    }

    void ProjectViewModel::closeProject() {
        logger_->info("Attempting to close project with ID: {}, name: {}", project_->getId(), project_->getName());
        if (!project_) {
            logger_->warn("Close project requested but no project is open.");
            return;
        }

        project_ = nullptr;
        emit projectClosed();
        logger_->info("Project closed successfully");
    }

    void ProjectViewModel::addNewProjectNode(
        const ProjectContext &context,
        const NodeType type,
        const std::string &name
    ) {
        logger_->info("Request to add new file '{}' to folder '{}'.", name, context.nodeId);
        const auto parentFolderOpt = project_->findNode(context.category, context.nodeId);

        if (!parentFolderOpt.has_value() || !parentFolderOpt.value()->isFolder()) {
            logger_->warn("Parent folder '{}' not found for new file '{}'.", context.nodeId, name);
            return;
        }

        auto parentFolder = dynamic_cast<NodeContainer *>(parentFolderOpt.value());

        std::string finalName = name;
        int counter = 1;
        while (parentFolder->hasChildWithName(finalName)) {
            finalName = name + "_" + std::to_string(counter++);
        }

        std::unique_ptr<ProjectNode> node;

        if (type == NodeType::FILE) {
            node = std::make_unique<FileNode>(
                idFactory_->create(),
                parentFolder,
                finalName,
                project_->getCategoryPath(context.category)
            );
        } else if (type == NodeType::FOLDER) {
            node = std::make_unique<NodeContainer>(
                idFactory_->create(),
                parentFolder,
                finalName
            );
        } else {
            logger_->warn("Unsupported node type: {} for new node '{}'.", toString(type), name);
            emit addNewProjectNodeFailed(tr("Failed to add new node: Unsupported node type."));
            return;
        }

        const auto addedNode = node.get();
        parentFolder->addChild(std::move(node));
        emit projectNodeAdded(addedNode, context.category);
        logger_->info(
            "Successfully added new node '{}' (ID: {}), type: {}, category: {} to folder '{}'.",
            name,
            addedNode->getId(),
            toString(context.nodeType),
            toString(context.category),
            context.parentId
        );
    }

    void ProjectViewModel::openFileNodeRequested(ProjectContext context) {
        logger_->info("Request to open file with ID: {}, category: {}", context.nodeId, toString(context.category));
        const auto nodeOpt = project_->findNode(context.category, context.nodeId);

        if (!nodeOpt.has_value() || !nodeOpt.value()->isFile()) {
            logger_->warn(
                "File with ID: {}, category: {} not found or not is file node for open request",
                context.nodeId,
                toString(context.category)
            );
            return;
        }

        emit openFileNode(dynamic_cast<const FileNode *>(nodeOpt.value()));
        logger_->info(
            "Open file request for ID: {}, category: {} emitted.",
            context.nodeId,
            toString(context.category)
        );
    }

    void ProjectViewModel::removeProjectNode(const ProjectContext &context) {
        logger_->info(
            "Request to remove node with ID: {}, category: {}, type: {}",
            context.nodeId,
            toString(context.category),
            toString(context.nodeType)
        );
        const auto nodeOpt = project_->findNode(context.category, context.nodeId);

        if (!nodeOpt.has_value() || !nodeOpt.value()->canBeRemoved()) {
            logger_->warn(
                "File with ID: {}, category: {}, type: {} not found or no be removed for remove",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );
            emit removeProjectNodeFailed(tr("Failed to remove node with ID: {}"));
            return;
        }

        nodeOpt.value()->getParent()->removeChild(context.nodeId);
        emit projectNodeRemoved(context.nodeId);
        logger_->info(
            "Successfully removed node with ID: {}, category: {}, type: {}.",
            context.nodeId,
            toString(context.category),
            toString(context.nodeType)
        );
    }

    void ProjectViewModel::renameProjectNode(const ProjectContext &context, const std::string &newName) {
        logger_->info(
            "Request to rename node with ID: {}, category: {}, type: {}",
            context.nodeId,
            toString(context.category),
            toString(context.nodeType)
        );
        const auto nodeOpt = project_->findNode(context.category, context.nodeId);

        if (!nodeOpt.has_value() || !nodeOpt.value()->canBeRenamed()) {
            logger_->warn(
                "File with ID: {}, category: {}, type: {} not found or no be renamed for rename",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );
            emit renameProjectNodeFailed(tr("Failed to rename node with ID: {}").toStdString());
            return;
        }

        if (newName == nodeOpt.value()->getName()) {
            logger_->info(
                "Node with ID: {}, category: {}, type: {} already has the name '{}', no rename needed.",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType),
                newName
            );
            return;
        }

        if (nodeOpt.value()->getParent()->hasChildWithName(newName)) {
            logger_->warn("A node with the name '{}' already exists.", newName);
            emit renameProjectNodeFailed(
                tr("A node with the name '%1' already exists.").arg(QString::fromStdString(newName)).toStdString()
            );
            return;
        }

        nodeOpt.value()->rename(newName);
        emit projectNodeRenamed(context.nodeId, newName);
        logger_->info(
            "Successfully renamed node with ID: {}, category: {}, type: {}, lastName: {}, newName: {}",
            context.nodeId,
            toString(context.category),
            toString(context.nodeType),
            nodeOpt.value()->getName(),
            newName
        );
    }

    void ProjectViewModel::copyProjectNode(const ProjectContext &context) {
        logger_->info(
            "Cuting node with id: {}, category: {}, type: {}",
            context.nodeId,
            toString(context.category),
            toString(context.nodeType)
        );
        const auto node = project_->findNode(context.category, context.nodeId);

        if (!node.has_value() || !node.value()->canBeCopied()) {
            logger_->warn(
                "Node with id: {}, category: {}, type: {} not found or not be copied",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );
            emit projectNodeCopiedFailed(tr("Failed to copy node with ID: {}"));
            return;
        }

        clipboard_.setCopy(node.value());
        emit projectNodeCopiedToClipboard();
    }

    void ProjectViewModel::cutProjectNode(const ProjectContext &context) {
        logger_->info(
            "Cutting node with id: {}, category: {}, type: {}",
            context.nodeId,
            toString(context.category),
            toString(context.nodeType)
        );
        const auto node = project_->findNode(context.category, context.nodeId);

        if (!node.has_value() || !node.value()->canBeMoved()) {
            logger_->warn(
                "Node with id: {}, category: {}, type: {} not found or not be cut",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );
            emit projectNodeCutFailed(tr("Failed to cut node with ID: {}"));
            return;
        }

        clipboard_.setCut(node.value());
        emit projectNodeCutToClipboard();
    }

    void ProjectViewModel::pasteProjectNode(const ProjectContext &context) {
        logger_->info(
            "Pasting node with id: {}, category: {}, type: {}",
            context.nodeId,
            toString(context.category),
            toString(context.nodeType)
        );

        if (clipboard_.isEmpty()) {
            logger_->warn(
                "Not pasting node with ID: {}, category: {}, type: {} clipboard is empty",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );
            return;
        }

        const auto nodeOpt = project_->findNode(context.category, context.nodeId);

        if (!nodeOpt.has_value()) {
            logger_->warn(
                "Node with ID: {}, category: {}, type: {} node not found",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );
            return;
        }

        ProjectNode *node = nodeOpt.value();
        if (node->getType() == NodeType::FILE) {
            logger_->info(
                "Node with ID: {}, category: {}, type: {} is a file, using it parent folder",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );

            node = node->getParent();

            if (!node || !node->isFolder()) {
                logger_->warn(
                    "Node with ID: {}, category: {}, type: {} parent not exist or is not a folder",
                    context.nodeId,
                    toString(context.category),
                    toString(context.nodeType)
                );
                return;
            }
        }

        const auto targetFolderOpt = node->getAsFolder();

        if (!targetFolderOpt.has_value()) {
            logger_->warn(
                "Invalid node parent with ID: {}, category: {}, type: {}",
                context.nodeId,
                toString(context.category),
                toString(context.nodeType)
            );
            return;
        }

        const auto targetFolder = targetFolderOpt.value();

        if (clipboard_.isCut()) {
            if (!clipboard_.getNode()->canBeMoved()) {
                logger_->warn(
                    "Node with ID: {}, category: {}, type: {} can not be moved",
                    context.nodeId,
                    toString(context.category),
                    toString(context.nodeType)
                );
                return;
            }

            const auto originalParent = clipboard_.getNode()->getParent();
            auto nodePtr = originalParent->releaseChild(clipboard_.getNode()->getId());
            const auto releasedNode = nodePtr.get();

            releasedNode->rename(createValidChildNameForFolder(targetFolder, releasedNode->getName()));
            targetFolder->addChild(std::move(nodePtr));
            emit projectNodeMoved(releasedNode);
            logger_->info(
                "Successfully cut node with ID: {}, category: {}, type: {} to folder with ID: {}, name: {}",
                releasedNode->getId(),
                toString(context.category),
                toString(releasedNode->getType()),
                targetFolder->getId(),
                targetFolder->getName()
            );
        } else if (clipboard_.isCopy()) {
            if (!clipboard_.getNode()->canBeCopied()) {
                logger_->warn(
                    "Node with ID: {}, category: {}, type: {} can not be copied",
                    context.nodeId,
                    toString(context.category),
                    toString(context.nodeType)
                );
                return;
            }

            const auto copyNode = clipboard_.getNode()->copy(idFactory_);
            copyNode->rename(createValidChildNameForFolder(targetFolder, copyNode->getName()));

            targetFolder->addChild(std::unique_ptr<ProjectNode>(copyNode));
            emit projectNodeCopied(clipboard_.getNode()->getId(), copyNode, context.category);
            logger_->info(
                "Successfully copy node with ID: {}, category: {}, type: {} to folder with ID: {}, name: {} and receive ID: {}",
                clipboard_.getNode()->getId(),
                toString(context.category),
                toString(clipboard_.getNode()->getType()),
                targetFolder->getId(),
                targetFolder->getName(),
                copyNode->getId()
            );
        }

        clipboard_.clear();
    }

    void ProjectViewModel::moveProjectNode(const std::string &sourceNodeId, const std::string &targetParentId) {
        logger_->info("Moving node with id: {} to parent with id: {}", sourceNodeId, targetParentId);

        auto sourceNodeOpt = project_->findNode(ProjectCategoryType::DIAGRAM, sourceNodeId);
        auto targetParentOpt = project_->findNode(ProjectCategoryType::DIAGRAM, targetParentId);

        if (!sourceNodeOpt.has_value() || !targetParentOpt.has_value()) {
            logger_->warn("Invalid source or target for move operation");
            return;
        }

        if (!targetParentOpt.value()->isFolder()) {
            logger_->info("Target parent is not a folder using its parent folder");
            targetParentOpt = targetParentOpt.value()->getParent();
        }

        if (!targetParentOpt.has_value() || !targetParentOpt.value()->isFolder()) {
            logger_->warn("Target parent is not a valid folder");
            return;
        }

        auto sourceNode = sourceNodeOpt.value();
        auto targetParent = dynamic_cast<NodeContainer *>(targetParentOpt.value());

        if (sourceNode->getParent() == targetParent) {
            logger_->info("Source node is already in the target folder");
            return;
        }

        auto currentParent = targetParent;
        while (currentParent) {
            if (currentParent == sourceNode) {
                logger_->warn("Cannot move a folder into one of its own descendants");
                return;
            }
            currentParent = currentParent->getParent();
        }

        auto originalParent = sourceNode->getParent();
        auto nodePtr = originalParent->releaseChild(sourceNodeId);
        auto releasedNode = nodePtr.get();

        releasedNode->rename(createValidChildNameForFolder(targetParent, releasedNode->getName()));
        targetParent->addChild(std::move(nodePtr));
        emit projectNodeMoved(releasedNode);
    }

    std::string ProjectViewModel::createValidChildNameForFolder(
        const NodeContainer *folder,
        const std::string &name
    ) {
        if (!folder->hasChildWithName(name)) {
            return name;
        }

        std::string baseName = name;
        int counter = 1;
        int padding = 0;

        std::size_t pos = name.find_last_not_of("0123456789");
        if (pos != std::string::npos && pos + 1 < name.size()) {
            const std::string numberPart = name.substr(pos + 1);
            baseName = name.substr(0, pos + 1);
            padding = static_cast<int>(numberPart.size());
            try {
                counter = std::stoi(numberPart);
            } catch (...) {
                counter = 1;
            }
        }

        std::string finalName = name;
        while (folder->hasChildWithName(finalName)) {
            std::ostringstream oss;
            if (!baseName.empty() && baseName.back() == '_') {
                oss << baseName << std::setw(padding) << std::setfill('0') << counter;
            } else if (std::isdigit(name.back())) {
                oss << baseName << std::setw(padding) << std::setfill('0') << counter;
            } else {
                oss << baseName << "_" << counter;
            }

            finalName = oss.str();
            ++counter;
        }

        return finalName;
    }
}
