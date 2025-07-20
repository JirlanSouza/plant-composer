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

namespace dp = domain::project;

namespace ui::project {
    class ProjectViewManager : public QObject {
        Q_OBJECT

    public:
        explicit ProjectViewManager(
            ProjectViewModel *projectViewModel,
            QWidget *parent = nullptr
        );

        ~ProjectViewManager() override;

        [[nodiscard]] QWidget *getView() const;

    private slots:
        void onCreateNewProjectRequested();

        void onTreeViewDoubleClicked(const QModelIndex &index);

        void onTreeViewContextMenuRequested(const QPoint &pos);

        void onAddNewDiagramTriggered() const;

        void onAddNewFolderTriggered() const;

        void onOpenTriggered() const;

        void onRenameTriggered() const;

        void onDeleteTriggered() const;

        void onItemReadyForEditing(const QModelIndex &index) const;

    private:
        ProjectViewModel *projectViewModel_;
        ProjectTreeModel *projectTreeModel_;
        ProjectTreeView *projectTreeView_;

        QAction *openAction_;
        QAction *renameAction_;
        QAction *deleteAction_;
        QAction *addDiagramAction_;
        QAction *addFolderAction_;

        QModelIndex currentItemIndex_;
        std::string currentItemId_;
        TreeItemTypes::TreeItemType currentItemType_;

        void createActions();
    };
}
