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
#include <QMessageBox>

#include "new_project_dialog.h"

namespace project {
    ProjectViewManager::ProjectViewManager(
        common::ILoggerFactory *loggerFactory,
        ProjectViewModel *projectViewModel,
        app_actions::ActionsManager *actionsManager,
        QWidget *parent
    ): QObject(parent),
        logger_(loggerFactory->getLogger("ProjectViewManager")),
        projectViewModel_(projectViewModel),
        actionsManager_(actionsManager) {
        projectTreeModel_ = new ProjectTreeModel(loggerFactory, projectViewModel, this);
        projectTreeView_ = new ProjectTreeView(projectTreeModel_, parent);

        createActions();

        connect(projectViewModel_, &ProjectViewModel::projectOpened, this, &ProjectViewManager::onProjectOpened);
        connect(
            projectViewModel_,
            &ProjectViewModel::openProjectFailed,
            this,
            &ProjectViewManager::onOpenProjectFailed
        );
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
        connect(projectViewModel_, &ProjectViewModel::nodeCopied, this, &ProjectViewManager::onNodeCopied);
        connect(projectViewModel_, &ProjectViewModel::nodeCut, this, &ProjectViewManager::onNodeCut);
    }

    ProjectViewManager::~ProjectViewManager() = default;

    QWidget *ProjectViewManager::getView() const {
        return projectTreeView_;
    }

    void ProjectViewManager::createActions() {
        newProjectAction_ = new QAction(QIcon::fromTheme("document-new"), tr("New Project"), this);
        newProjectAction_->setIconVisibleInMenu(true);
        connect(newProjectAction_, &QAction::triggered, this, &ProjectViewManager::onCreateNewProjectTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::File, newProjectAction_);
        actionsManager_->addAction(app_actions::ActionGroupType::ToolbarFile, newProjectAction_);

        openProjectAction_ = new QAction(QIcon::fromTheme("document-open"), tr("Open Project"), this);
        openProjectAction_->setIconVisibleInMenu(true);
        connect(openProjectAction_, &QAction::triggered, this, &ProjectViewManager::onOpenProjectTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::File, openProjectAction_);
        actionsManager_->addAction(app_actions::ActionGroupType::ToolbarFile, openProjectAction_);

        saveProjectAction_ = new QAction(QIcon::fromTheme("document-save"), tr("Save Project"), this);
        saveProjectAction_->setIconVisibleInMenu(true);
        saveProjectAction_->setEnabled(false);
        connect(saveProjectAction_, &QAction::triggered, projectViewModel_, &ProjectViewModel::saveProject);
        actionsManager_->addAction(app_actions::ActionGroupType::File, saveProjectAction_);
        actionsManager_->addAction(app_actions::ActionGroupType::ToolbarFile, saveProjectAction_);

        closeProjectAction_ = new QAction(QIcon::fromTheme("document-close"), tr("Close Project"), this);
        closeProjectAction_->setIconVisibleInMenu(true);
        closeProjectAction_->setEnabled(false);
        connect(closeProjectAction_, &QAction::triggered, projectViewModel_, &ProjectViewModel::closeProject);
        actionsManager_->addAction(app_actions::ActionGroupType::File, closeProjectAction_);

        newDiagramAction_ = new QAction(QIcon::fromTheme("document-new"), tr("New Diagram"), this);
        newDiagramAction_->setIconVisibleInMenu(true);
        connect(newDiagramAction_, &QAction::triggered, this, &ProjectViewManager::onAddNewDiagramTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, newDiagramAction_);

        newFolderAction_ = new QAction(QIcon::fromTheme("folder-new"), tr("New Folder"), this);
        newFolderAction_->setIconVisibleInMenu(true);
        connect(newFolderAction_, &QAction::triggered, this, &ProjectViewManager::onAddNewFolderTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, newFolderAction_);

        openAction_ = new QAction(QIcon::fromTheme("document-open"), tr("Open"), this);
        openAction_->setIconVisibleInMenu(true);
        connect(openAction_, &QAction::triggered, this, &ProjectViewManager::onOpenTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::File, openAction_);

        renameAction_ = new QAction(QIcon::fromTheme("edit-rename"), tr("Rename"), this);
        renameAction_->setIconVisibleInMenu(true);
        connect(renameAction_, &QAction::triggered, this, &ProjectViewManager::onRenameTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, renameAction_);

        deleteAction_ = new QAction(QIcon::fromTheme("edit-delete"), tr("Delete"), this);
        deleteAction_->setIconVisibleInMenu(true);
        connect(deleteAction_, &QAction::triggered, this, &ProjectViewManager::onDeleteTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, deleteAction_);

        copyAction_ = new QAction(QIcon::fromTheme("edit-copy"), tr("Copy"), this);
        copyAction_->setIconVisibleInMenu(true);
        connect(copyAction_, &QAction::triggered, this, &ProjectViewManager::onCopyTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, copyAction_);

        cutAction_ = new QAction(QIcon::fromTheme("edit-cut"), tr("Cut"), this);
        cutAction_->setIconVisibleInMenu(true);
        connect(cutAction_, &QAction::triggered, this, &ProjectViewManager::onCutTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, cutAction_);

        pasteAction_ = new QAction(QIcon::fromTheme("edit-paste"), tr("Paste"), this);
        pasteAction_->setIconVisibleInMenu(true);
        connect(pasteAction_, &QAction::triggered, this, &ProjectViewManager::onPasteTriggered);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, pasteAction_);
        pasteAction_->setEnabled(false);
    }

    void ProjectViewManager::onCreateNewProjectTriggered() const {
        logger_->info("User triggered 'New Project' action.");
        NewProjectDialog newProjectDialog(getView());
        if (newProjectDialog.exec() == QDialog::Accepted) {
            projectViewModel_->createNewProject(
                newProjectDialog.getProjectName().toStdString(),
                newProjectDialog.getProjectDescription().toStdString(),
                newProjectDialog.getProjectAuthor().toStdString(),
                newProjectDialog.getProjectParentDirectory().toStdString()
            );
        }
    }

    void ProjectViewManager::onOpenProjectTriggered() const {
        logger_->info("User triggered 'Open Project' action.");
        const QString projectPath = QFileDialog::getOpenFileName(
            this->getView(),
            tr("Open Project"),
            QDir::homePath(),
            tr("Plant Composer Project Files (*.pcp);;All Files (*)")
        );

        if (!projectPath.isEmpty()) {
            projectViewModel_->openProject(projectPath.toStdString());
        }
    }

    void ProjectViewManager::onProjectOpened() const {
        logger_->info("Project opened signal received. Expanding tree.");
        projectTreeView_->expandAll();
        projectTreeView_->setFocus();
        saveProjectAction_->setEnabled(true);
        closeProjectAction_->setEnabled(true);
    }

    void ProjectViewManager::onOpenProjectFailed(const QString &errorMessage) const {
        logger_->error("Project open failed: {}", errorMessage.toStdString());
        const auto messageBox = new QMessageBox(
            QMessageBox::Critical,
            tr("Open Project Failed"),
            errorMessage,
            QMessageBox::Ok,
            this->getView()
        );
        messageBox->exec();
        messageBox->deleteLater();
    }

    void ProjectViewManager::onTreeViewDoubleClicked(const QModelIndex &index) {
        if (!index.isValid()) return;

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        currentItemId_ = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();

        logger_->info("User double-clicked item ID: {}, Type: {}", currentItemId_, static_cast<int>(type));

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

        logger_->info("User right-clicked item ID: {}, Type: {}", currentItemId_, static_cast<int>(currentItemType_));

        QMenu menu;
        menu.setMinimumWidth(220);
        if (currentItemType_ == TreeItemTypes::DIAGRAM_ROOT_FOLDER ||
            currentItemType_ == TreeItemTypes::DIAGRAM_FOLDER) {
            menu.addAction(newDiagramAction_);
            menu.addAction(newFolderAction_);
            menu.addSeparator();
            menu.addAction(copyAction_);
            menu.addAction(cutAction_);
            menu.addAction(pasteAction_);
            menu.addSeparator();
            menu.addAction(renameAction_);
            menu.addAction(deleteAction_);
        } else if (currentItemType_ == TreeItemTypes::DIAGRAM_FILE) {
            menu.addAction(openAction_);
            menu.addSeparator();
            menu.addAction(copyAction_);
            menu.addAction(cutAction_);
            menu.addSeparator();
            menu.addAction(renameAction_);
            menu.addAction(deleteAction_);
        }

        if (!menu.isEmpty()) {
            menu.exec(projectTreeView_->viewport()->mapToGlobal(pos));
        }
    }

    void ProjectViewManager::onAddNewDiagramTriggered() const {
        logger_->info("User triggered 'Add New Diagram' for parent: {}", currentItemId_);
        projectViewModel_->addNewDiagram(currentItemId_, "New Diagram");
    }

    void ProjectViewManager::onAddNewFolderTriggered() const {
        logger_->info("User triggered 'Add New Folder' for parent: {}", currentItemId_);
        projectViewModel_->addNewDiagramFolder(currentItemId_, "New Folder");
    }

    void ProjectViewManager::onOpenTriggered() const {
        logger_->info("User triggered 'Open' for item: {}", currentItemId_);
        projectViewModel_->openDiagramRequested(currentItemId_);
    }

    void ProjectViewManager::onRenameTriggered() const {
        logger_->info("User triggered 'Rename' for item: {}", currentItemId_);
        if (currentItemType_ == TreeItemTypes::DIAGRAM_FILE || currentItemType_ == TreeItemTypes::DIAGRAM_FOLDER) {
            projectTreeView_->edit(currentItemIndex_);
        }
    }

    void ProjectViewManager::onDeleteTriggered() const {
        logger_->info("User triggered 'Delete' for item: {}", currentItemId_);
        if (currentItemType_ == TreeItemTypes::DIAGRAM_FILE) {
            projectViewModel_->removeDiagram(currentItemId_);
        } else if (currentItemType_ == TreeItemTypes::DIAGRAM_FOLDER) {
            projectViewModel_->removeDiagramFolder(currentItemId_);
        }
    }

    void ProjectViewManager::onItemReadyForEditing(const QModelIndex &index) const {
        logger_->info("Item ready for editing at index: {}", index.row());
        projectTreeView_->edit(index);
    }

    void ProjectViewManager::onCopyTriggered() const {
        logger_->info("User triggered 'Copy' for item: {}", currentItemId_);
        projectViewModel_->copy(currentItemId_);
    }

    void ProjectViewManager::onCutTriggered() const {
        logger_->info("User triggered 'Cut' for item: {}", currentItemId_);
        projectViewModel_->cut(currentItemId_);
    }

    void ProjectViewManager::onPasteTriggered() const {
        logger_->info("User triggered 'Paste' for item: {}", currentItemId_);
        projectViewModel_->paste(currentItemId_);
    }

    void ProjectViewManager::onNodeCopied() const {
        pasteAction_->setEnabled(true);
    }

    void ProjectViewManager::onNodeCut() const {
        pasteAction_->setEnabled(true);
    }
}
