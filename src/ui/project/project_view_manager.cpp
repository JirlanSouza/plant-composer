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

#include "project_view_manager.h"

#include <QMenu>

namespace ui::project {
    ProjectViewManager::ProjectViewManager(ProjectViewModel *projectViewModel, QWidget *parent): QObject(parent),
        projectViewModel_(projectViewModel) {
        projectTreeModel_ = new ProjectTreeModel(projectViewModel, this);
        projectTreeView_ = new ProjectTreeView(projectTreeModel_, projectViewModel, parent);
        newDiagramDialog_ = new NewDiagramDialog(projectTreeView_);

        connect(projectTreeView_, &ProjectTreeView::doubleClicked, this, &ProjectViewManager::onTreeViewDoubleClicked);
        connect(
            projectTreeView_,
            &ProjectTreeView::customContextMenuRequested,
            this,
            &ProjectViewManager::onTreeViewContextMenuRequested
        );
        connect(
            newDiagramDialog_,
            &NewDiagramDialog::diagramNameEntered,
            projectViewModel_,
            &ProjectViewModel::addNewDiagram
        );
    }

    ProjectViewManager::~ProjectViewManager() = default;

    QWidget *ProjectViewManager::getView() const {
        return projectTreeView_;
    }

    void ProjectViewManager::onTreeViewDoubleClicked(const QModelIndex &index) const {
        if (!index.isValid()) {
            return;
        }

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto typeData = item->data(ProjectTreeRole::ITEM_TYPE_ROLE);

        if (!typeData.isValid()) {
            return;
        }

        switch (typeData.value<TreeItemTypes::TreeItemType>()) {
            case TreeItemTypes::ADD_DIAGRAM_ACTION_ITEM: {
                const auto parent = item->parent();
                if (!parent) {
                    return;
                }

                newDiagramDialog_->execFor("", "");
            }
            default: ;
        }
    }

    void ProjectViewManager::onTreeViewContextMenuRequested(const QPoint &pos) const {
        const auto index = projectTreeView_->indexAt(pos);
        if (!index.isValid()) {
            return;
        }

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto typeData = item->data(ProjectTreeRole::ITEM_TYPE_ROLE);
        qDebug() << "ProjectViewManager::onTreeViewContextMenuRequested with type: "
                << static_cast<int>(typeData.value<TreeItemTypes::TreeItemType>());

        if (!typeData.isValid()) {
            return;
        }

        switch (typeData.value<TreeItemTypes::TreeItemType>()) {
            case TreeItemTypes::DIAGRAM_ROOT_FOLDER:
            case TreeItemTypes::DIAGRAM_FOLDER: {
                const auto folderIdData = item->data(ProjectTreeRole::ITEM_ID_ROLE);

                if (!folderIdData.isValid()) {
                    return;
                }

                const auto folderId = folderIdData.toString().toStdString();


                QMenu menu;
                auto addDiagramAction = menu.addAction(tr("Add Diagram"));
                auto addSubfolderAction = menu.addAction(tr("Add folder"));

                connect(
                    addDiagramAction,
                    &QAction::triggered,
                    this,
                    [this, folderId]() {
                        newDiagramDialog_->execFor(folderId, "");
                    }
                );

                connect(
                    addSubfolderAction,
                    &QAction::triggered,
                    [this, folderId]() {
                        projectViewModel_->addNewDiagramFolder(folderId, "New Folder " + this->projectTreeModel_->rowCount());
                    }
                );

                menu.exec(projectTreeView_->viewport()->mapToGlobal(pos));
            }
            default: {
                qDebug() << "ProjectViewManager::onTreeViewContextMenuRequested with type: " << static_cast<int>(
                    typeData.value<TreeItemTypes::TreeItemType>());
            };
        }
    }
}
