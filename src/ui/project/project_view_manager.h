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
#include <QAction>

#include "project/project_tree/project_tree_model.h"
#include "project/project_tree/project_tree_view.h"
#include "project/project_view_model.h"
#include "domain/common/iuser_notifier.h"
#include "ui/actions_manager/actions_manager.h"

namespace project {
    class ProjectViewManager : public QObject {
        Q_OBJECT

    public:
        explicit ProjectViewManager(
            common::ILoggerFactory *loggerFactory,
            common::IUserNotifier *notifier,
            ProjectViewModel *projectViewModel,
            app_actions::ActionsManager *actionsManager,
            QWidget *parent = nullptr
        );

        ~ProjectViewManager() override;

        [[nodiscard]] QWidget *getView() const;

    private slots:
        void onCreateNewProjectTriggered() const;

        void onProjectCreateFailed(const std::string &parentDirectory) const;

        void onOpenProjectTriggered() const;

        void onProjectOpened() const;

        void onProjectOpenFailed(const std::string &path) const;

        void onTreeViewItemSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

        void onTreeViewDoubleClicked(const QModelIndex &index) const;

        void onTreeViewContextMenuRequested(const QPoint &pos);

        void onAddNewDiagramTriggered() const;

        void onAddNewFolderTriggered() const;

        void onOpenActionTriggered() const;

        void onRenameTriggered() const;

        void onDeleteActionTriggered() const;

        void onItemReadyForEditing(const QModelIndex &index) const;

        void onCopyActionTriggered();

        void onCutActionTriggered();

        void onPasteActionTriggered();

        void onNodeCopiedToClipboard() const;

        void onNodeCutToClipboard() const;

        void onProjectNodeRenameFailed(const std::string &message) const;

    private:
        std::unique_ptr<common::Ilogger> logger_;
        common::IUserNotifier *notifier_;
        ProjectViewModel *projectViewModel_;
        app_actions::ActionsManager *actionsManager_;
        ProjectTreeModel *projectTreeModel_;
        ProjectTreeView *projectTreeView_;

        QAction *newProjectAction_;
        QAction *openProjectAction_;
        QAction *saveProjectAction_;
        QAction *closeProjectAction_;
        QAction *newDiagramAction_;
        QAction *newFolderAction_;
        QAction *openAction_;
        QAction *renameAction_;
        QAction *deleteAction_;
        QAction *copyAction_;
        QAction *cutAction_;
        QAction *pasteAction_;

        std::optional<ProjectContext> contextMenuContext_;

        void handleNodeOpening(const ProjectContext &context) const;

        void handleNodeDeletion(const ProjectContext &context) const;

        void createActions();

        QAction *createAction(
            const QString &name,
            const QIcon::ThemeIcon &themeIcon,
            const QString &tooltip,
            const std::function<void()> &handler
        );
    };
}
