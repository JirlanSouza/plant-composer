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
#include <QInputDialog>

namespace ui::project {
    ProjectViewManager::ProjectViewManager(ProjectViewModel *projectViewModel, QWidget *parent): QObject(parent),
        projectViewModel_(projectViewModel) {
        projectTreeModel_ = new ProjectTreeModel(projectViewModel, this);
        projectTreeView_ = new ProjectTreeView(projectTreeModel_, parent);

        createActions();

        connect(projectTreeView_, &ProjectTreeView::activated, this, &ProjectViewManager::onTreeViewDoubleClicked);
        connect(
            projectTreeView_,
            &ProjectTreeView::customContextMenuRequested,
            this,
            &ProjectViewManager::onTreeViewContextMenuRequested
        );
    }

    ProjectViewManager::~ProjectViewManager() = default;

    QWidget *ProjectViewManager::getView() const {
        return projectTreeView_;
    }

    void ProjectViewManager::createActions() {
        addDiagramAction_ = new QAction(tr("Add Diagram"), this);
        addFolderAction_ = new QAction(tr("Add Folder"), this);
        openAction_ = new QAction(tr("Open"), this);
        renameAction_ = new QAction(tr("Rename"), this);
        deleteAction_ = new QAction(tr("Delete"), this);

        connect(addDiagramAction_, &QAction::triggered, this, &ProjectViewManager::onAddNewDiagramTriggered);
        connect(addFolderAction_, &QAction::triggered, this, &ProjectViewManager::onAddNewFolderTriggered);
        connect(openAction_, &QAction::triggered, this, &ProjectViewManager::onOpenTriggered);
        connect(renameAction_, &QAction::triggered, this, &ProjectViewManager::onRenameTriggered);
        connect(deleteAction_, &QAction::triggered, this, &ProjectViewManager::onDeleteTriggered);
    }

    void ProjectViewManager::onTreeViewDoubleClicked(const QModelIndex &index) {
        if (!index.isValid()) return;

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        currentItemId_ = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();

        if (type == TreeItemTypes::ADD_DIAGRAM_ACTION_ITEM) {
            const auto parent = item->parent();
            if (parent) {
                currentItemId_ = parent->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
                onAddNewDiagramTriggered();
            }
        } else if (type == TreeItemTypes::DIAGRAM_FILE) {
            projectViewModel_->openDiagramRequested(currentItemId_);
        } else if (type == TreeItemTypes::DIAGRAM_FOLDER || type == TreeItemTypes::DIAGRAM_ROOT_FOLDER) {
            projectTreeView_->toggleExpanded(index);
        }
    }

    void ProjectViewManager::onTreeViewContextMenuRequested(const QPoint &pos) {
        const auto index = projectTreeView_->indexAt(pos);
        if (!index.isValid()) return;

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        currentItemId_ = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();

        QMenu menu;
        if (type == TreeItemTypes::DIAGRAM_ROOT_FOLDER || type == TreeItemTypes::DIAGRAM_FOLDER) {
            menu.addAction(addDiagramAction_);
            menu.addAction(addFolderAction_);
            menu.addSeparator();
            menu.addAction(renameAction_);
            menu.addAction(deleteAction_);
        } else if (type == TreeItemTypes::DIAGRAM_FILE) {
            menu.addAction(openAction_);
            menu.addSeparator();
            menu.addAction(renameAction_);
            menu.addAction(deleteAction_);
        }

        if (!menu.isEmpty()) {
            menu.exec(projectTreeView_->viewport()->mapToGlobal(pos));
        }
    }

    void ProjectViewManager::onAddNewDiagramTriggered() {
        bool ok;
        QString name = QInputDialog::getText(
            projectTreeView_,
            tr("Add Diagram"),
            tr("Diagram name:"),
            QLineEdit::Normal,
            "",
            &ok
        );
        if (ok && !name.isEmpty()) {
            projectViewModel_->addNewDiagram(currentItemId_, name.toStdString());
        }
    }

    void ProjectViewManager::onAddNewFolderTriggered() {
        bool ok;
        QString name = QInputDialog::getText(
            projectTreeView_,
            tr("Add Folder"),
            tr("Folder name:"),
            QLineEdit::Normal,
            "",
            &ok
        );
        if (ok && !name.isEmpty()) {
            projectViewModel_->addNewDiagramFolder(currentItemId_, name.toStdString());
        }
    }

    void ProjectViewManager::onOpenTriggered() {
        projectViewModel_->openDiagramRequested(currentItemId_);
    }

    void ProjectViewManager::onRenameTriggered() {
        // TODO: Implement rename logic using currentItemId_
    }

    void ProjectViewManager::onDeleteTriggered() {
        // TODO: Implement delete logic using currentItemId_
    }
}
