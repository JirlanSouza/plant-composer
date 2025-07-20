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

#include "new_project_dialog.h"

namespace ui::project {
    ProjectViewManager::ProjectViewManager(
        ProjectViewModel *projectViewModel,
        uam::ActionsManager *actionsManager,
        QWidget *parent
    ): QObject(parent),
        projectViewModel_(projectViewModel),
        actionsManager_(actionsManager) {
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
        connect(
            projectTreeModel_,
            &ProjectTreeModel::itemReadyForEditing,
            this,
            &ProjectViewManager::onItemReadyForEditing
        );
    }

    ProjectViewManager::~ProjectViewManager() = default;

    QWidget *ProjectViewManager::getView() const {
        return projectTreeView_;
    }

    void ProjectViewManager::createActions() {
        newProjectAction_ = new QAction(QIcon::fromTheme("document-new"), tr("New Project"), this);
        connect(newProjectAction_, &QAction::triggered, this, &ProjectViewManager::onCreateNewProjectTriggered);
        actionsManager_->addAction(uam::ActionGroupType::File, newProjectAction_);
        actionsManager_->addAction(uam::ActionGroupType::ToolbarFile, newProjectAction_);

        addDiagramAction_ = new QAction(tr("Add Diagram"), this);
        connect(addDiagramAction_, &QAction::triggered, this, &ProjectViewManager::onAddNewDiagramTriggered);
        actionsManager_->addAction(uam::ActionGroupType::Edit, addDiagramAction_);

        addFolderAction_ = new QAction(tr("Add Folder"), this);
        connect(addFolderAction_, &QAction::triggered, this, &ProjectViewManager::onAddNewFolderTriggered);
        actionsManager_->addAction(uam::ActionGroupType::Edit, addFolderAction_);

        openAction_ = new QAction(tr("Open"), this);
        connect(openAction_, &QAction::triggered, this, &ProjectViewManager::onOpenTriggered);
        actionsManager_->addAction(uam::ActionGroupType::File, openAction_);

        renameAction_ = new QAction(tr("Rename"), this);
        connect(renameAction_, &QAction::triggered, this, &ProjectViewManager::onRenameTriggered);
        actionsManager_->addAction(uam::ActionGroupType::Edit, renameAction_);

        deleteAction_ = new QAction(tr("Delete"), this);
        connect(deleteAction_, &QAction::triggered, this, &ProjectViewManager::onDeleteTriggered);
        actionsManager_->addAction(uam::ActionGroupType::Edit, deleteAction_);
    }

    void ProjectViewManager::onCreateNewProjectTriggered() {
        const auto newProjectDialog = new NewProjectDialog(this->getView());
        connect(
            newProjectDialog,
            &NewProjectDialog::accepted,
            [this, newProjectDialog]() {
                const auto name = newProjectDialog->getProjectName();
                const auto description = newProjectDialog->getProjectDescription();
                const auto author = newProjectDialog->getProjectAuthor();
                const auto path = newProjectDialog->getProjectPath();

                projectViewModel_->createNewProject(
                    name.toStdString(),
                    description.toStdString(),
                    author.toStdString(),
                    path.toStdString()
                );
            }
        );
        newProjectDialog->exec();
        newProjectDialog->deleteLater();
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
        currentItemIndex_ = projectTreeView_->indexAt(pos);
        if (!currentItemIndex_.isValid()) return;

        const auto item = projectTreeModel_->itemFromIndex(currentItemIndex_);
        currentItemType_ = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        currentItemId_ = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();

        QMenu menu;
        menu.setMinimumWidth(220);
        if (currentItemType_ == TreeItemTypes::DIAGRAM_ROOT_FOLDER ||
            currentItemType_ == TreeItemTypes::DIAGRAM_FOLDER) {
            menu.addAction(addDiagramAction_);
            menu.addAction(addFolderAction_);
            menu.addSeparator();
            menu.addAction(renameAction_);
            menu.addAction(deleteAction_);
        } else if (currentItemType_ == TreeItemTypes::DIAGRAM_FILE) {
            menu.addAction(openAction_);
            menu.addSeparator();
            menu.addAction(renameAction_);
            menu.addAction(deleteAction_);
        }

        if (!menu.isEmpty()) {
            menu.exec(projectTreeView_->viewport()->mapToGlobal(pos));
        }
    }

    void ProjectViewManager::onAddNewDiagramTriggered() const {
        projectViewModel_->addNewDiagram(currentItemId_, "New Diagram");
    }

    void ProjectViewManager::onAddNewFolderTriggered() const {
        projectViewModel_->addNewDiagramFolder(currentItemId_, "New Folder");
    }

    void ProjectViewManager::onOpenTriggered() const {
        projectViewModel_->openDiagramRequested(currentItemId_);
    }

    void ProjectViewManager::onRenameTriggered() const {
        if (currentItemType_ == TreeItemTypes::DIAGRAM_FILE || currentItemType_ == TreeItemTypes::DIAGRAM_FOLDER) {
            projectTreeView_->edit(currentItemIndex_);
        }
    }

    void ProjectViewManager::onDeleteTriggered() const {
        if (currentItemType_ == TreeItemTypes::DIAGRAM_FILE) {
            projectViewModel_->removeDiagram(currentItemId_);
        } else if (currentItemType_ == TreeItemTypes::DIAGRAM_FOLDER) {
            projectViewModel_->removeDiagramFolder(currentItemId_);
        }
    }

    void ProjectViewManager::onItemReadyForEditing(const QModelIndex &index) const {
        projectTreeView_->edit(index);
    }
}
