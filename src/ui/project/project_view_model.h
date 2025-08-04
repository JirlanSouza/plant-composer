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

#include "domain/project/project_loader.h"
#include "domain/project/model/project.h"
#include "domain/common/id_factory.h"
#include "domain/common/ilogger.h"
#include "domain/common/ilogger_factory.h"
#include "clipboard.h"

namespace project {
    class ProjectViewModel : public QObject {
        Q_OBJECT

    public:
        explicit ProjectViewModel(
            common::ILoggerFactory *loggerFactory,
            common::IDFactory *idFactory,
            IProjectLoader *projectLoader,
            QObject *parent = nullptr
        );

        [[nodiscard]] Project *getProject() const;

        [[nodiscard]] bool hasOpenedProject() const;

    public slots:
        void createNewProject(
            const std::string &name,
            const std::string &description,
            const std::string &author,
            const std::string &parentDirectory
        );

        void openProject(const std::string &path);

        void saveProject() const;

        void closeProject();

        void addNewProjectNode(const project::ProjectContext &context, NodeType type, const std::string &name);

        void openFileNodeRequested(project::ProjectContext context);

        void removeProjectNode(const project::ProjectContext &context);

        void renameProjectNode(const project::ProjectContext &context, const std::string &newName);

        void copyProjectNode(const project::ProjectContext &context);

        void cutProjectNode(const project::ProjectContext &context);

        void pasteProjectNode(const project::ProjectContext &context);

        void moveProjectNode(const std::string &sourceNodeId, const std::string &targetParentId);


    signals:
        void projectOpened();

        void projectCreateFailed(const std::string &parentDirectory);

        void projectOpenFailed(const std::string &path);

        void projectClosed();

        void projectNodeAdded(const project::ProjectNode *node, ProjectCategoryType categoryType);

        void addNewProjectNodeFailed(const QString &errorMessage);

        void openFileNode(const project::FileNode *fileNode);

        void openFileNodeFailed(const QString &errorMessage);

        void projectNodeRemoved(const std::string &nodeId);

        void removeProjectNodeFailed(const QString &errorMessage);

        void projectNodeRenamed(const std::string &nodeId, const std::string &newName);

        void renameProjectNodeFailed(const std::string &message);

        void projectNodeCopiedToClipboard();

        void projectNodeCopiedFailed(const QString &errorMessage);

        void projectNodeCutToClipboard();

        void projectNodeCutFailed(const QString &errorMessage);

        void projectNodeCopied(
            const std::string &copiedNodeId,
            const project::ProjectNode *copyNode,
            ProjectCategoryType categoryType
        );

        void projectNodeMoved(const project::ProjectNode *node);

    private:
        std::unique_ptr<common::Ilogger> logger_;
        common::IDFactory *idFactory_;
        IProjectLoader *projectLoader_;
        std::unique_ptr<Project> project_{nullptr};
        Clipboard clipboard_;

        static std::string createValidChildNameForFolder(const NodeContainer *folder, const std::string &name);
    };
}
