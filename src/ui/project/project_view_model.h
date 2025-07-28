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

        void addNewProjectNode(const ProjectContext &context, const std::string &name);

        void openFileNodeRequested(ProjectContext context);

        void removeProjectNode(const ProjectContext &context);

        void renameProjectNode(const ProjectContext &context, const std::string &newName);

        void copyProjectNode(const ProjectContext &context);

        void cutProjectNode(const ProjectContext &context);

        void pasteProjectNode(const ProjectContext &context);


    signals:
        void projectOpened();

        void openProjectFailed(const QString &string);

        void projectClosed();

        void projectNodeAdded(const project::ProjectNode *node);

        void addNewProjectNodeFailed(const QString &errorMessage);

        void openFileNode(const project::FileNode *fileNode);

        void openFileNodeFailed(const QString &errorMessage);

        void projectNodeRemoved(const std::string &nodeId);

        void removeProjectNodeFailed(const QString &errorMessage);

        void projectNodeRenamed(const std::string &nodeId, const std::string &newName);

        void renameProjectNodeFailed(const QString &errorMessage);

        void projectNodeCopied();

        void projectNodeCopiedFailed(const QString &errorMessage);

        void projectNodeCut();

        void projectNodeCutFailed(const QString &errorMessage);

        void projectNodePastedAsCopy(const project::ProjectNode *node);

        void projectNodePastedAsCut(const project::ProjectNode *node);

    private:
        std::unique_ptr<common::Ilogger> logger_;
        common::IDFactory *idFactory_;
        IProjectLoader *projectLoader_;
        std::unique_ptr<Project> project_{nullptr};

        enum class ClipboardMode {
            NONE, COPY, CUT
        };

        struct ClipboardItem {
            ClipboardMode mode = ClipboardMode::NONE;
            const ProjectNode *node = nullptr;
        };

        ClipboardItem clipboard_;
    };
}
