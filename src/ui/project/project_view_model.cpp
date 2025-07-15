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

#include <iostream>

namespace ui::project {
    ProjectViewModel::ProjectViewModel(IDFactory *idFactory, dp::Project *project, QObject *parent)
        : QObject(parent), idFactory_(idFactory), project_(project) {
    }

    dp::Project *ProjectViewModel::getProject() const {
        return project_;
    }

    void ProjectViewModel::addNewDiagram(const std::string &parentFolderId, const std::string &name) {
        dp::NodeContainer<dp::DiagramMetadata> *parentFolder;

        if (parentFolderId == project_->diagrams()->getId()) {
            parentFolder = project_->diagrams();
        } else {
            auto parentFolderOpt = project_->diagrams()->getFolder(parentFolderId);
            if (!parentFolderOpt.has_value()) {
                return;
            }

            parentFolder = parentFolderOpt.value();
        }


        auto diagram = std::make_unique<dp::DiagramMetadata>(
            idFactory_->newId(),
            parentFolder,
            name,
            "diagrams/" + name + ".fbs"
        );

        emit diagramAdded(diagram.get());
        parentFolder->addChild(std::move(diagram));
    }

    void ProjectViewModel::addNewDiagramFolder(const std::string &parentFolderId, const std::string &name) {
        dp::NodeContainer<dp::DiagramMetadata> *parentFolder;

        if (parentFolderId == project_->diagrams()->getId()) {
            parentFolder = project_->diagrams();
        } else {
            auto parentFolderOpt = project_->diagrams()->getFolder(parentFolderId);
            if (!parentFolderOpt.has_value()) {
                return;
            }

            parentFolder = parentFolderOpt.value();
        }

        auto newFolder = std::make_unique<dp::NodeContainer<dp::DiagramMetadata> >(
            idFactory_->newId(),
            parentFolder,
            name
        );

        emit diagramFolderAdded(newFolder.get());
        parentFolder->addChild(std::move(newFolder));
    }

    void ProjectViewModel::openDiagramRequested(const std::string &diagramId) {
        const auto diagramOpt = project_->diagrams()->getFile(diagramId);

        if (!diagramOpt.has_value()) {
            return;
        }

        emit openDiagram(diagramOpt.value());
    }
}
