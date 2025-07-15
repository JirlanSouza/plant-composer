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
        projectTreeView_ = new ProjectTreeView(projectTreeModel_, projectViewModel, parent);

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

    void ProjectViewManager::onTreeViewDoubleClicked(const QModelIndex &index) const {
        if (!index.isValid()) {
            return;
        }

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();

        if (type == TreeItemTypes::ADD_DIAGRAM_ACTION_ITEM) {
            const auto parent = item->parent();
            if (parent) {
                const auto parentId = parent->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
                onAddNewDiagramTriggered(parentId);
            }
        } else if (type == TreeItemTypes::DIAGRAM_FILE) {
            const auto itemId = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
            projectViewModel_->openDiagramRequested(itemId);
        } else if (type == TreeItemTypes::DIAGRAM_FOLDER || type == TreeItemTypes::DIAGRAM_ROOT_FOLDER) {
            bool expanded = projectTreeView_->isExpanded(index);
            projectTreeView_->setExpanded(index, !expanded);
        }
    }

    void ProjectViewManager::onTreeViewContextMenuRequested(const QPoint &pos) const {
        const auto index = projectTreeView_->indexAt(pos);
        if (!index.isValid()) {
            return;
        }

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        const auto itemId = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();

        QMenu menu;
        if (type == TreeItemTypes::DIAGRAM_ROOT_FOLDER || type == TreeItemTypes::DIAGRAM_FOLDER) {
            menu.addAction(tr("Add Diagram"), this, [this, itemId]() { onAddNewDiagramTriggered(itemId); });
            menu.addAction(tr("Add folder"), this, [this, itemId]() { onAddNewFolderTriggered(itemId); });
        } else if (type == TreeItemTypes::DIAGRAM_FILE) {
            // ... (diagram-specific actions)
        }

        if (!menu.isEmpty()) {
            menu.exec(projectTreeView_->viewport()->mapToGlobal(pos));
        }
    }

    void ProjectViewManager::onAddNewDiagramTriggered(const std::string &parentId) const {
        bool ok;
        const QString name = QInputDialog::getText(
            projectTreeView_,
            tr("Add Diagram"),
            tr("Diagram name:"),
            QLineEdit::Normal,
            "",
            &ok
        );
        if (ok && !name.isEmpty()) {
            projectViewModel_->addNewDiagram(parentId, name.toStdString());
        }
    }

    void ProjectViewManager::onAddNewFolderTriggered(const std::string &parentId) const {
        bool ok;
        const QString name = QInputDialog::getText(
            projectTreeView_,
            tr("Add Folder"),
            tr("Folder name:"),
            QLineEdit::Normal,
            "",
            &ok
        );
        if (ok && !name.isEmpty()) {
            projectViewModel_->addNewDiagramFolder(parentId, name.toStdString());
        }
    }
}
