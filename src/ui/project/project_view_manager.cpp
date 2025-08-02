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
#include "domain/common/iuser_notifier.h"

namespace project {
    ProjectViewManager::ProjectViewManager(
        common::ILoggerFactory *loggerFactory,
        common::IUserNotifier *notifier,
        ProjectViewModel *projectViewModel,
        app_actions::ActionsManager *actionsManager,
        QWidget *parent
    ): QObject(parent),
        logger_(loggerFactory->getLogger("ProjectViewManager")),
        projectViewModel_(projectViewModel),
        actionsManager_(actionsManager),
        notifier_(notifier) {
        projectTreeModel_ = new ProjectTreeModel(loggerFactory, projectViewModel, this);
        projectTreeView_ = new ProjectTreeView(projectTreeModel_, parent);
        const auto projectSelectionModel = new QItemSelectionModel(projectTreeModel_, this);
        projectTreeView_->setSelectionModel(projectSelectionModel);

        createActions();

        connect(
            projectViewModel_,
            &ProjectViewModel::projectCreateFailed,
            this,
            &ProjectViewManager::onProjectCreateFailed
        );
        connect(projectViewModel_, &ProjectViewModel::projectOpened, this, &ProjectViewManager::onProjectOpened);
        connect(
            projectViewModel_,
            &ProjectViewModel::projectOpenFailed,
            this,
            &ProjectViewManager::onProjectOpenFailed
        );
        connect(
            projectSelectionModel,
            &QItemSelectionModel::selectionChanged,
            this,
            &ProjectViewManager::onTreeViewItemSelectionChanged
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
        connect(projectViewModel_, &ProjectViewModel::projectNodeCopied, this, &ProjectViewManager::onNodeCopied);
        connect(projectViewModel_, &ProjectViewModel::projectNodeCut, this, &ProjectViewManager::onNodeCut);
    }

    ProjectViewManager::~ProjectViewManager() = default;

    QWidget *ProjectViewManager::getView() const {
        return projectTreeView_;
    }

    void ProjectViewManager::createActions() {
        newProjectAction_ = createAction(
            tr("New Project"),
            QIcon::ThemeIcon::DocumentNew,
            tr("Create new project"),
            [this] { onCreateNewProjectTriggered(); }
        );
        actionsManager_->addAction(
            app_actions::ActionGroupType::File,
            newProjectAction_,
            app_actions::ShortcutId::NewProject
        );
        actionsManager_->addAction(app_actions::ActionGroupType::ToolbarFile, newProjectAction_);

        openProjectAction_ = createAction(
            tr("Open Project"),
            QIcon::ThemeIcon::DocumentOpen,
            tr("Open existent project"),
            [this] { onOpenProjectTriggered(); }
        );
        actionsManager_->addAction(
            app_actions::ActionGroupType::File,
            openProjectAction_,
            app_actions::ShortcutId::OpenProject
        );
        actionsManager_->addAction(app_actions::ActionGroupType::ToolbarFile, openProjectAction_);

        saveProjectAction_ = createAction(
            tr("Save Project"),
            QIcon::ThemeIcon::DocumentSave,
            tr("Save project"),
            [this] { projectViewModel_->saveProject(); }
        );
        saveProjectAction_->setEnabled(false);
        actionsManager_->addAction(
            app_actions::ActionGroupType::File,
            saveProjectAction_,
            app_actions::ShortcutId::SaveProject
        );
        actionsManager_->addAction(app_actions::ActionGroupType::ToolbarFile, saveProjectAction_);

        closeProjectAction_ = createAction(
            tr("Close Project"),
            QIcon::ThemeIcon::NThemeIcons,
            tr("Close current project"),
            [this] { projectViewModel_->closeProject(); }
        );
        closeProjectAction_->setEnabled(false);
        actionsManager_->addAction(app_actions::ActionGroupType::File, closeProjectAction_);

        newDiagramAction_ = createAction(
            tr("New Diagram"),
            QIcon::ThemeIcon::DocumentNew,
            tr("Create new diagram"),
            [this] { onAddNewDiagramTriggered(); }
        );
        actionsManager_->addAction(
            app_actions::ActionGroupType::Edit,
            newDiagramAction_,
            app_actions::ShortcutId::NewFile
        );

        newFolderAction_ = createAction(
            tr("New Folder"),
            QIcon::ThemeIcon::FolderNew,
            tr("Create new folder"),
            [this] { onAddNewFolderTriggered(); }
        );
        actionsManager_->addAction(
            app_actions::ActionGroupType::Edit,
            newFolderAction_,
            app_actions::ShortcutId::NewFolder
        );

        openAction_ = createAction(
            tr("Open"),
            QIcon::ThemeIcon::DocumentOpen,
            tr("Open item"),
            [this] { onOpenActionTriggered(); }
        );
        actionsManager_->addAction(app_actions::ActionGroupType::File, openAction_);

        renameAction_ = createAction(
            tr("Rename"),
            QIcon::ThemeIcon::NThemeIcons,
            tr("Rename item"),
            [this] { onRenameTriggered(); }
        );
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, renameAction_, app_actions::ShortcutId::Rename);

        deleteAction_ = createAction(
            tr("Delete"),
            QIcon::ThemeIcon::EditDelete,
            tr("Delete item"),
            [this] { onDeleteActionTriggered(); }
        );
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, deleteAction_, app_actions::ShortcutId::Delete);

        copyAction_ = createAction(
            tr("Copy"),
            QIcon::ThemeIcon::EditCopy,
            tr("Copy item"),
            [this] { onCopyActionTriggered(); }
        );
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, copyAction_, app_actions::ShortcutId::Copy);

        cutAction_ = createAction(
            tr("Cut"),
            QIcon::ThemeIcon::EditCut,
            tr("Cut item"),
            [this] { onCutActionTriggered(); }
        );
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, cutAction_, app_actions::ShortcutId::Cut);

        pasteAction_ = createAction(
            tr("Paste"),
            QIcon::ThemeIcon::EditPaste,
            tr("Paste item"),
            [this] { onPasteActionTriggered(); }
        );
        pasteAction_->setEnabled(false);
        actionsManager_->addAction(app_actions::ActionGroupType::Edit, pasteAction_, app_actions::ShortcutId::Paste);
    }

    QAction *ProjectViewManager::createAction(
        const QString &name,
        const QIcon::ThemeIcon &themeIcon,
        const QString &tooltip,
        const std::function<void()> &handler
    ) {
        QIcon icon;
        if (themeIcon != QIcon::ThemeIcon::NThemeIcons) {
            icon = QIcon::fromTheme(themeIcon);
        } else {
            logger_->warn("Icon not found in theme for action: {}", name.toStdString());
            icon = QIcon();
        }

        auto *action = new QAction(icon, name, this);
        action->setToolTip(tooltip);
        action->setIconVisibleInMenu(true);
        connect(action, &QAction::triggered, handler);
        return action;
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

    void ProjectViewManager::onProjectCreateFailed(const std::string &parentDirectory) const {
        const auto message = tr(
            "Could not create project at the specified directory: %1. Please ensure the directory is valid and you have the necessary permissions."
        ).arg(QString::fromStdString(parentDirectory));
        notifier_->showAlert(
            tr("Create Project Failed").toStdString(),
            message.toStdString(),
            common::NotificationLevel::Error
        );
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

    void ProjectViewManager::onProjectOpenFailed(const std::string &path) const {
        const auto message = tr(
            "Could not open the project file at the specified path: %1. Please ensure the file is valid and you have the necessary permissions."
        ).arg(QString::fromStdString(path));
        notifier_->showAlert(
            tr("Open Project Failed").toStdString(),
            message.toStdString(),
            common::NotificationLevel::Error
        );
    }

    void ProjectViewManager::onTreeViewItemSelectionChanged(
        const QItemSelection &selected,
        const QItemSelection &deselected
    ) {
        if (selected.indexes().isEmpty()) {
            logger_->info("User deselected all items on project tree");
            return;
        }

        const auto item = projectTreeModel_->itemFromIndex(selected.indexes().first());
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        const auto id = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
        const auto parentId = item->parent()
                                  ? item->parent()->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString()
                                  : "";
        const auto category = TreeItemTypes::toProjectCategory(type);
        const auto nodeType = TreeItemTypes::toNodeType(type);

        if (!category.has_value() || !nodeType.has_value()) {
            contextMenuContext_ = std::nullopt;
            return;
        }

        contextMenuContext_ = {category.value(), parentId, nodeType.value(), id};

        logger_->info("User selected item ID: {}, Type: {}", id, static_cast<int>(type));
    }


    void ProjectViewManager::onTreeViewDoubleClicked(const QModelIndex &index) const {
        if (!index.isValid()) return;

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();

        if (type == TreeItemTypes::ADD_DIAGRAM_ACTION_ITEM) {
            logger_->info("AddNewDiagram item clicked, creating new diagram in parent folder");
            const auto parent = item->parent();
            if (parent) {
                const auto parentId = parent->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
                const auto parentType = parent->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<
                    TreeItemTypes::TreeItemType>();
                const auto parentCategory = TreeItemTypes::toProjectCategory(parentType);
                if (parentCategory.has_value()) {
                    projectViewModel_->addNewProjectNode(
                        {parentCategory.value(), "", NodeType::FILE, parentId},
                        NodeType::FILE,
                        "New Diagram"
                    );
                }
            }
            return;
        }

        const auto id = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
        const auto parentId = item->parent()
                                  ? item->parent()->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString()
                                  : "";
        const auto category = TreeItemTypes::toProjectCategory(type);
        const auto nodeType = TreeItemTypes::toNodeType(type);

        if (!category.has_value() || !nodeType.has_value()) {
            logger_->warn("Could not determine project context for double-clicked item");
            return;
        }

        handleNodeOpening({category.value(), parentId, nodeType.value(), id});
    }

    void ProjectViewManager::onTreeViewContextMenuRequested(const QPoint &pos) {
        const auto index = projectTreeView_->indexAt(pos);
        if (!index.isValid()) {
            contextMenuContext_ = std::nullopt;
            return;
        }

        const auto item = projectTreeModel_->itemFromIndex(index);
        const auto type = item->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        const auto id = item->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
        const auto parentId = item->parent()
                                  ? item->parent()->data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString()
                                  : "";
        const auto category = TreeItemTypes::toProjectCategory(type);
        const auto nodeType = TreeItemTypes::toNodeType(type);

        if (!category.has_value() || !nodeType.has_value()) {
            contextMenuContext_ = std::nullopt;
            return;
        }

        contextMenuContext_ = {category.value(), parentId, nodeType.value(), id};

        logger_->info("User right-clicked item ID: {}, Type: {}", id, static_cast<int>(type));

        QMenu menu;
        menu.setMinimumWidth(220);
        if (type == TreeItemTypes::DIAGRAM_ROOT_FOLDER) {
            menu.addAction(newDiagramAction_);
            menu.addAction(newFolderAction_);
            menu.addSeparator();
            menu.addAction(pasteAction_);
        } else if (type == TreeItemTypes::DIAGRAM_FOLDER) {
            menu.addAction(newDiagramAction_);
            menu.addAction(newFolderAction_);
            menu.addSeparator();
            menu.addAction(copyAction_);
            menu.addAction(cutAction_);
            menu.addAction(pasteAction_);
            menu.addSeparator();
            menu.addAction(renameAction_);
            menu.addAction(deleteAction_);
        } else if (type == TreeItemTypes::DIAGRAM_FILE) {
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
        if (!contextMenuContext_.has_value() || contextMenuContext_.value().nodeType != NodeType::FOLDER) return;
        logger_->info("User triggered 'Add New Diagram' for parent: {}", contextMenuContext_.value().nodeId);
        projectViewModel_->addNewProjectNode(contextMenuContext_.value(), NodeType::FILE, "New Diagram");
    }

    void ProjectViewManager::onAddNewFolderTriggered() const {
        if (!contextMenuContext_.has_value()) return;
        logger_->info("User triggered 'Add New Folder' for parent: {}", contextMenuContext_.value().nodeId);
        projectViewModel_->addNewProjectNode(contextMenuContext_.value(), NodeType::FOLDER, "New Folder");
    }

    void ProjectViewManager::onOpenActionTriggered() const {
        if (!contextMenuContext_.has_value()) return;
        handleNodeOpening(contextMenuContext_.value());
    }

    void ProjectViewManager::onRenameTriggered() const {
        if (!contextMenuContext_.has_value()) return;
        logger_->info("User triggered 'Rename' for item: {}", contextMenuContext_.value().nodeId);
        const auto index = projectTreeView_->indexAt(
            projectTreeView_->visualRect(projectTreeView_->currentIndex()).center()
        );
        projectTreeView_->edit(index);
    }

    void ProjectViewManager::onDeleteActionTriggered() const {
        if (!contextMenuContext_.has_value()) return;
        handleNodeDeletion(contextMenuContext_.value());
    }

    void ProjectViewManager::onItemReadyForEditing(const QModelIndex &index) const {
        logger_->info("Item ready for editing at index: {}", index.row());
        projectTreeView_->edit(index);
    }

    void ProjectViewManager::onCopyActionTriggered() {
        if (!contextMenuContext_.has_value()) return;
        logger_->info("User triggered 'Copy' for item: {}", contextMenuContext_.value().nodeId);
        projectViewModel_->copyProjectNode(contextMenuContext_.value());
    }

    void ProjectViewManager::onCutActionTriggered() {
        if (!contextMenuContext_.has_value()) return;
        logger_->info("User triggered 'Cut' for item: {}", contextMenuContext_.value().nodeId);
        projectViewModel_->cutProjectNode(contextMenuContext_.value());
    }

    void ProjectViewManager::onPasteActionTriggered() {
        if (!contextMenuContext_.has_value()) return;
        logger_->info("User triggered 'Paste' for item: {}", contextMenuContext_.value().nodeId);
        projectViewModel_->pasteProjectNode(contextMenuContext_.value());
    }

    void ProjectViewManager::onNodeCopied() const {
        pasteAction_->setEnabled(true);
    }

    void ProjectViewManager::onNodeCut() const {
        pasteAction_->setEnabled(true);
    }

    void ProjectViewManager::handleNodeOpening(const ProjectContext &context) const {
        logger_->info("Handling node opening for item: {}", context.nodeId);
        if (context.nodeType == NodeType::FILE) {
            projectViewModel_->openFileNodeRequested(context);
        }
    }

    void ProjectViewManager::handleNodeDeletion(const ProjectContext &context) const {
        logger_->info("Handling node deletion for item: {}", context.nodeId);
        projectViewModel_->removeProjectNode(context);
    }
}
