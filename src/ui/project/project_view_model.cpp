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
            emit openProjectFailed(QString("Failed to create project in path: ").append(parentDirectory));
            logger_->error("Failed to create project: {}", name);
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
            emit openProjectFailed(QString("Failed to open project from path: ").append(path));
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

    void ProjectViewModel::addNewDiagram(const std::string &parentFolderId, const std::string &name) {
        logger_->info("Request to add new diagram '{}' to folder '{}'.", name, parentFolderId);
        NodeContainer *parentFolder;

        if (parentFolderId == project_->diagrams()->getId()) {
            parentFolder = project_->diagrams();
        } else {
            auto parentFolderOpt = project_->diagrams()->getFolder(parentFolderId);
            if (!parentFolderOpt.has_value()) {
                logger_->warn("Parent folder '{}' not found for new diagram '{}'.", parentFolderId, name);
                return;
            }

            parentFolder = parentFolderOpt.value();
        }


        auto diagram = std::make_unique<DiagramMetadata>(
            idFactory_->create(),
            parentFolder,
            name,
            "diagrams/" + name + ".fbs"
        );

        auto addedDiagram = diagram.get();
        parentFolder->addChild(std::move(diagram));
        emit diagramAdded(addedDiagram);
        logger_->info(
            "Successfully added new diagram '{}' (ID: {}) to folder '{}'.",
            name,
            addedDiagram->getId(),
            parentFolderId
        );
    }

    void ProjectViewModel::addNewDiagramFolder(const std::string &parentFolderId, const std::string &name) {
        logger_->info("Request to add new folder '{}' to folder '{}'.", name, parentFolderId);
        NodeContainer *parentFolder;

        if (parentFolderId == project_->diagrams()->getId()) {
            parentFolder = project_->diagrams();
        } else {
            auto parentFolderOpt = project_->diagrams()->getFolder(parentFolderId);
            if (!parentFolderOpt.has_value()) {
                logger_->warn("Parent folder '{}' not found for new folder '{}'.", parentFolderId, name);
                return;
            }

            parentFolder = parentFolderOpt.value();
        }

        auto newFolder = std::make_unique<NodeContainer>(
            idFactory_->create(),
            parentFolder,
            name
        );

        auto addedFolder = newFolder.get();
        parentFolder->addChild(std::move(newFolder));
        emit diagramFolderAdded(addedFolder);
        logger_->info(
            "Successfully added new folder '{}' (ID: {}) to folder '{}'.",
            name,
            addedFolder->getId(),
            parentFolderId
        );
    }

    void ProjectViewModel::openDiagramRequested(const std::string &diagramId) {
        logger_->info("Request to open diagram with ID: {}", diagramId);
        const auto diagramOpt = project_->diagrams()->getFile(diagramId);
        if (!diagramOpt.has_value()) {
            logger_->warn("Diagram with ID '{}' not found for open request.", diagramId);
            return;
        }

        emit openDiagram(dynamic_cast<const DiagramMetadata *>(diagramOpt.value()));
        logger_->info("Open diagram request for ID: {} emitted.", diagramId);
    }

    void ProjectViewModel::removeDiagram(const std::string &diagramId) {
        logger_->info("Request to remove diagram with ID: {}", diagramId);
        const auto diagramOpt = project_->diagrams()->getFile(diagramId);
        if (!diagramOpt.has_value()) {
            logger_->warn("Diagram with ID '{}' not found for remove request.", diagramId);
            return;
        }

        diagramOpt.value()->getParent()->removeChild(diagramId);
        emit diagramRemoved(diagramId);
        logger_->info("Successfully removed diagram with ID: {}.", diagramId);
    }

    void ProjectViewModel::removeDiagramFolder(const std::string &folderId) {
        logger_->info("Request to remove folder with ID: {}", folderId);
        const auto folderOpt = project_->diagrams()->getFolder(folderId);
        if (!folderOpt.has_value()) {
            logger_->warn("Folder with ID '{}' not found for remove request.", folderId);
            return;
        }

        folderOpt.value()->getParent()->removeChild(folderId);
        emit diagramFolderRemoved(folderId);
        logger_->info("Successfully removed folder with ID: {}.", folderId);
    }

    void ProjectViewModel::renameDiagram(const std::string &diagramId, const std::string &newName) {
        logger_->info("Request to rename diagram with ID: {} to '{}'.", diagramId, newName);
        const auto diagramOpt = project_->diagrams()->getFile(diagramId);
        if (!diagramOpt.has_value()) {
            logger_->warn("Diagram with ID '{}' not found for rename request.", diagramId);
            return;
        }

        diagramOpt.value()->rename(newName);
        emit diagramRenamed(diagramId, newName);
        logger_->info("Successfully renamed diagram with ID: {} to '{}'.", diagramId, newName);
    }

    void ProjectViewModel::renameDiagramFolder(const std::string &folderId, const std::string &newName) {
        logger_->info("Request to rename folder with ID: {} to '{}'.", folderId, newName);
        const auto folderOpt = project_->diagrams()->getFolder(folderId);
        if (!folderOpt.has_value()) {
            logger_->warn("Folder with ID '{}' not found for rename request.", folderId);
            return;
        }

        folderOpt.value()->rename(newName);
        emit diagramFolderRenamed(folderId, newName);
        logger_->info("Successfully renamed folder with ID: {} to '{}'.", folderId, newName);
    }

    void ProjectViewModel::copy(const std::string &itemId) {
        logger_->info("Copying item with id: {}", itemId);
        const auto item = project_->findNode(itemId);
        if (!item) {
            logger_->warn("Item with id: {} not found", itemId);
            return;
        }

        clipboard_.mode = ClipboardMode::COPY;
        clipboard_.node = item;
        emit nodeCopied();
    }

    void ProjectViewModel::cut(const std::string &itemId) {
        logger_->info("Cutting item with id: {}", itemId);
        const auto item = project_->findNode(itemId);
        if (!item) {
            logger_->warn("Item with id: {} not found", itemId);
            return;
        }

        clipboard_.mode = ClipboardMode::CUT;
        clipboard_.node = item;
        emit nodeCut();
    }

    void ProjectViewModel::paste(const std::string &targetId) {
        logger_->info("Pasting item to target with id: {}", targetId);
        if (clipboard_.mode == ClipboardMode::NONE || !clipboard_.node) {
            logger_->warn("Clipboard is empty");
            return;
        }

        const auto target = project_->findNode(targetId);
        if (!target || !target->isFolder()) {
            logger_->warn("Target with id: {} not found or is not a folder", targetId);
            return;
        }

        auto targetFolder = target->getAsFolder().value();

        if (clipboard_.mode == ClipboardMode::CUT) {
            auto originalParent = clipboard_.node->getParent();
            auto node = originalParent->releaseChild(clipboard_.node->getId());
            auto releasedNode = node.get();
            targetFolder->addChild(std::move(node));
            emit diagramRemoved(releasedNode->getId());
            emit diagramAdded(static_cast<const DiagramMetadata *>(releasedNode));
        } else if (clipboard_.mode == ClipboardMode::COPY) {
            // TODO: Implement deep copy of the node
        }

        clipboard_.mode = ClipboardMode::NONE;
        clipboard_.node = nullptr;
    }
}
