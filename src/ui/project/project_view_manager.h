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

#include "project_tree/project_tree_model.h"
#include "project_tree/project_tree_view.h"
#include "project_view_model.h"
#include "project_tree/project_tree_item_type.h"
#include "ui/actions_manager/actions_manager.h"

namespace dp = domain::project;
namespace uam = ui::actions_manager;

namespace ui::project {
    class ProjectViewManager : public QObject {
        Q_OBJECT

    public:
        explicit ProjectViewManager(
            domain::Ilogger *logger,
            ProjectViewModel *projectViewModel,
            uam::ActionsManager *actionsManager,
            QWidget *parent = nullptr
        );

        ~ProjectViewManager() override;

        [[nodiscard]] QWidget *getView() const;

    private slots:
        void onCreateNewProjectTriggered() const;

        void onOpenProjectTriggered() const;

        void onProjectOpened() const;

        void onOpenProjectFailed(const QString &errorMessage) const;

        void onTreeViewDoubleClicked(const QModelIndex &index);

        void onTreeViewContextMenuRequested(const QPoint &pos);

        void onAddNewDiagramTriggered() const;

        void onAddNewFolderTriggered() const;

        void onOpenTriggered() const;

        void onRenameTriggered() const;

        void onDeleteTriggered() const;

        void onItemReadyForEditing(const QModelIndex &index) const;

    private:
        domain::Ilogger *logger_;
        ProjectViewModel *projectViewModel_;
        uam::ActionsManager *actionsManager_;
        ProjectTreeModel *projectTreeModel_;
        ProjectTreeView *projectTreeView_;

        QAction *newProjectAction_;
        QAction *openProjectAction_;
        QAction *saveProjectAction_;
        QAction *closeProjectAction_;
        QAction *addDiagramAction_;
        QAction *addFolderAction_;
        QAction *openAction_;
        QAction *renameAction_;
        QAction *deleteAction_;

        QModelIndex currentItemIndex_;
        std::string currentItemId_;
        TreeItemTypes::TreeItemType currentItemType_;

        void createActions();
    };
}
