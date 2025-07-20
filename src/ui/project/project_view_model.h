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

#include <QObject>

#include "domain/project/model/project.h"
#include "domain/shared/id_factory.h"

using domain::IDFactory;
namespace dp = domain::project;

namespace ui::project {
    class ProjectViewModel : public QObject {
        Q_OBJECT

    public:
        explicit ProjectViewModel(IDFactory *idFactory, QObject *parent = nullptr);

        [[nodiscard]] dp::Project *getProject() const;

        [[nodiscard]] bool hasOpenedProject() const;

    public slots:
        void createNewProject(
            const std::string &name,
            const std::string &description,
            const std::string &author,
            const std::string &path
        );

        void addNewDiagram(const std::string &parentFolderId, const std::string &name);

        void addNewDiagramFolder(const std::string &parentFolderId, const std::string &name);

        void openDiagramRequested(const std::string &diagramId);

        void removeDiagram(const std::string &diagramId);

        void removeDiagramFolder(const std::string &folderId);

        void renameDiagram(const std::string &diagramId, const std::string &newName);

        void renameDiagramFolder(const std::string &folderId, const std::string &newName);


    signals:
        void projectOpened();

        void diagramAdded(const domain::project::DiagramMetadata *diagram);

        void diagramFolderAdded(const domain::project::NodeContainer<domain::project::DiagramMetadata> *folder);

        void openDiagram(const domain::project::DiagramMetadata *diagram);

        void diagramRemoved(const std::string &diagramId);

        void diagramFolderRemoved(const std::string &folderId);

        void diagramRenamed(const std::string &diagramId, const std::string &newName);

        void diagramFolderRenamed(const std::string &folderId, const std::string &newName);

    private:
        IDFactory *idFactory_;
        dp::Project *project_{nullptr};
    };
}
