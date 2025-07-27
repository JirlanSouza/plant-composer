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

namespace ui::project {
    ProjectViewModel::ProjectViewModel(
        common::ILoggerFactory *loggerFactory,
        common::IDFactory *idFactory,
        domain::project::IProjectLoader *projectLoader,
        QObject *parent
    )
        : QObject(parent), logger_(loggerFactory->getLogger("ProjectViewModel")), idFactory_(idFactory),
        projectLoader_(projectLoader) {
    }

    dp::Project *ProjectViewModel::getProject() const {
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
        auto projectOpt = projectLoader_->createNewProject(name, description, author, parentDirectory);
        if (!projectOpt.has_value()) {
            emit openProjectFailed(QString("Failed to create project in path: ").append(parentDirectory));
            return;
        }

        project_ = std::move(projectOpt.value());
        emit projectOpened();
    }

    void ProjectViewModel::openProject(const std::string &path) {
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
        logger_->info("Project opened successfully from path: {}", path);
        emit projectOpened();
    }

    void ProjectViewModel::saveProject() const {
        if (!project_) return;

        projectLoader_->saveProject(*project_);
    }

    void ProjectViewModel::closeProject() {
        logger_->info("Closing projecct with ID: {}, name: {}", project_->getId(), project_->getName());
        project_ = nullptr;
        emit projectClosed();
        logger_->info("Project closed successfully");
    }

    void ProjectViewModel::addNewDiagram(const std::string &parentFolderId, const std::string &name) {
        dp::NodeContainer *parentFolder;

        if (parentFolderId == project_->diagrams()->getId()) {
            parentFolder = project_->diagrams();
        } else {
            auto parentFolderOpt = project_->diagrams()->getFolder(parentFolderId);
            if (!parentFolderOpt.has_value()) return;

            parentFolder = parentFolderOpt.value();
        }


        auto diagram = std::make_unique<dp::DiagramMetadata>(
            idFactory_->create(),
            parentFolder,
            name,
            "diagrams/" + name + ".fbs"
        );

        emit diagramAdded(diagram.get());
        parentFolder->addChild(std::move(diagram));
    }

    void ProjectViewModel::addNewDiagramFolder(const std::string &parentFolderId, const std::string &name) {
        dp::NodeContainer *parentFolder;

        if (parentFolderId == project_->diagrams()->getId()) {
            parentFolder = project_->diagrams();
        } else {
            auto parentFolderOpt = project_->diagrams()->getFolder(parentFolderId);
            if (!parentFolderOpt.has_value()) return;

            parentFolder = parentFolderOpt.value();
        }

        auto newFolder = std::make_unique<dp::NodeContainer>(
            idFactory_->create(),
            parentFolder,
            name
        );

        emit diagramFolderAdded(newFolder.get());
        parentFolder->addChild(std::move(newFolder));
    }

    void ProjectViewModel::openDiagramRequested(const std::string &diagramId) {
        const auto diagramOpt = project_->diagrams()->getFile(diagramId);
        if (!diagramOpt.has_value()) return;

        emit openDiagram(dynamic_cast<const dp::DiagramMetadata *>(diagramOpt.value()));
    }

    void ProjectViewModel::removeDiagram(const std::string &diagramId) {
        const auto diagramOpt = project_->diagrams()->getFile(diagramId);
        if (!diagramOpt.has_value()) return;

        diagramOpt.value()->getParent()->removeChild(diagramId);
        emit diagramRemoved(diagramId);
    }

    void ProjectViewModel::removeDiagramFolder(const std::string &folderId) {
        const auto folderOpt = project_->diagrams()->getFolder(folderId);
        if (!folderOpt.has_value()) return;

        folderOpt.value()->getParent()->removeChild(folderId);
        emit diagramFolderRemoved(folderId);
    }

    void ProjectViewModel::renameDiagram(const std::string &diagramId, const std::string &newName) {
        const auto diagramOpt = project_->diagrams()->getFile(diagramId);
        if (!diagramOpt.has_value()) return;

        diagramOpt.value()->rename(newName);
        emit diagramRenamed(diagramId, newName);
    }

    void ProjectViewModel::renameDiagramFolder(const std::string &folderId, const std::string &newName) {
        const auto folderOpt = project_->diagrams()->getFolder(folderId);
        if (!folderOpt.has_value()) return;

        folderOpt.value()->rename(newName);
        emit diagramFolderRenamed(folderId, newName);
    }
}
