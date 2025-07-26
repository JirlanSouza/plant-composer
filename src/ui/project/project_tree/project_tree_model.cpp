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

#include "project_tree_model.h"

#include <QMimeData>

#include "project_tree_item_type.h"

namespace ui::project {
    ProjectTreeModel::ProjectTreeModel(
        domain::Ilogger *logger,
        ProjectViewModel *projectViewModel,
        QObject *parent
    ) : QStandardItemModel(parent), logger_(logger), projectViewModel_(projectViewModel) {
        connect(projectViewModel_, &ProjectViewModel::projectOpened, this, &ProjectTreeModel::buildModel);
        connect(projectViewModel_, &ProjectViewModel::projectClosed, this, &ProjectTreeModel::onProjectClosed);
        connect(projectViewModel_, &ProjectViewModel::diagramAdded, this, &ProjectTreeModel::onDiagramAdded);
        connect(
            projectViewModel_,
            &ProjectViewModel::diagramFolderAdded,
            this,
            &ProjectTreeModel::onDiagramFolderAdded
        );
        connect(projectViewModel_, &ProjectViewModel::diagramRemoved, this, &ProjectTreeModel::onDiagramRemoved);
        connect(
            projectViewModel_,
            &ProjectViewModel::diagramFolderRemoved,
            this,
            &ProjectTreeModel::onDiagramFolderRemoved
        );
        connect(
            projectViewModel_,
            &ProjectViewModel::diagramRenamed,
            this,
            &ProjectTreeModel::onDiagramRenamed
        );
        connect(
            projectViewModel_,
            &ProjectViewModel::diagramFolderRenamed,
            this,
            &ProjectTreeModel::onDiagramFolderRenamed
        );
    }

    bool ProjectTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
        if (role != Qt::EditRole) {
            return QStandardItemModel::setData(index, value, role);
        }

        const auto type = index.data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        const auto itemId = index.data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
        const auto newName = value.toString().toStdString();

        if (type == TreeItemTypes::DIAGRAM_FILE) {
            projectViewModel_->renameDiagram(itemId, newName);
        } else if (type == TreeItemTypes::DIAGRAM_FOLDER) {
            projectViewModel_->renameDiagramFolder(itemId, newName);
        }

        return false;
    }

    void ProjectTreeModel::buildModel() {
        logger_->info("Building project tree model");
        logger_->info("Cleaning up previous model data");
        clearModel();
        setColumnCount(1);

        logger_->info("Creating project root item");
        auto *rootItem = new QStandardItem(
            getIconForType(TreeItemTypes::TreeItemType::PROJECT_ROOT),
            tr("Project: %1").arg(QString::fromStdString(projectViewModel_->getProject()->getName()))
        );
        rootItem->setFlags(Qt::ItemIsEnabled);
        rootItem->setData(TreeItemTypes::TreeItemType::PROJECT_ROOT, ProjectTreeRole::ITEM_TYPE_ROLE);
        appendRow(rootItem);

        logger_->info("Building diagrams root item");
        const auto *diagramsRoot = projectViewModel_->getProject()->diagrams();
        auto *diagramsRootItem = new QStandardItem(
            getIconForType(TreeItemTypes::TreeItemType::DIAGRAM_ROOT_FOLDER),
            QString::fromStdString(diagramsRoot->getName())
        );
        diagramsRootItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        diagramsRootItem->setData(TreeItemTypes::TreeItemType::DIAGRAM_ROOT_FOLDER, ProjectTreeRole::ITEM_TYPE_ROLE);
        diagramsRootItem->setData(stdStringToVariant(diagramsRoot->getId()), ProjectTreeRole::ITEM_ID_ROLE);
        rootItem->appendRow(diagramsRootItem);
        itemMap_[diagramsRoot->getId()] = diagramsRootItem;

        logger_->info("Building addNewDiagram item");
        auto *addNewDiagramItem = new QStandardItem(
            getIconForType(TreeItemTypes::TreeItemType::ADD_DIAGRAM_ACTION_ITEM),
            tr("Add New Diagram")
        );
        addNewDiagramItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        addNewDiagramItem->setData(
            TreeItemTypes::TreeItemType::ADD_DIAGRAM_ACTION_ITEM,
            ProjectTreeRole::ITEM_TYPE_ROLE
        );
        diagramsRootItem->appendRow(addNewDiagramItem);
        itemMap_["add_new_diagram"] = addNewDiagramItem;

        populateFolder(diagramsRootItem, diagramsRoot, TreeItemTypes::TreeItemType::DIAGRAM_FOLDER);
    }

    void ProjectTreeModel::clearModel() {
        logger_->info("Clearing project tree model");
        clear();
        itemMap_.clear();
    }

    void ProjectTreeModel::populateFolder(
        QStandardItem *parentItem,
        const dp::NodeContainer *folder,
        const TreeItemTypes::TreeItemType type
    ) {
        logger_->info(
            "Populating folder with ID: {}, name: {}, children: {}",
            folder->getId(),
            folder->getName(),
            folder->getChildren().size()
        );
        for (const auto *item: folder->getChildren()) {
            logger_->info("Before");
            if (item->isFile()) {
                logger_->info("Before 2");
                appendItem(parentItem, dynamic_cast<const dp::FileNode *>(item), TreeItemTypes::DIAGRAM_FILE);
            } else if (item->isFolder()) {
                appendFolder(parentItem, dynamic_cast<const dp::NodeContainer *>(item), type);
            }
        }
        logger_->info(
            "Seccess populating folder with ID: {}, name: {}, children: {}",
            folder->getId(),
            folder->getName(),
            folder->getChildren().size()
        );
    }

    void ProjectTreeModel::appendItem(
        QStandardItem *parent,
        const dp::FileNode *file,
        const TreeItemTypes::TreeItemType type
    ) {
        logger_->info("Appending project tree item with ID: {}, name: {}", file->getId(), file->getName());
        auto *diagramItem = new QStandardItem(
            getIconForType(type),
            QString::fromStdString(file->getName())
        );
        diagramItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
        diagramItem->setData(type, ProjectTreeRole::ITEM_TYPE_ROLE);
        diagramItem->setEditable(file->canBeRenamed());
        diagramItem->setData(stdStringToVariant(file->getId()), ProjectTreeRole::ITEM_ID_ROLE);
        parent->appendRow(diagramItem);
        itemMap_[file->getId()] = diagramItem;
        logger_->info(
            "Successfully appended project tree item with ID: {}, name: {}",
            file->getId(),
            file->getName()
        );
    }

    void ProjectTreeModel::appendFolder(
        QStandardItem *parent,
        const dp::NodeContainer *folder,
        const TreeItemTypes::TreeItemType type
    ) {
        logger_->info(
            "Appeding project tree folder with ID: {}, name: {}, children: {}",
            folder->getId(),
            folder->getName(),
            folder->getChildren().size()
        );
        auto *folderItem = new QStandardItem(getIconForType(type), QString::fromStdString(folder->getName()));
        folderItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        folderItem->setEditable(folder->canBeRenamed());
        folderItem->setData(type, ProjectTreeRole::ITEM_TYPE_ROLE);
        folderItem->setData(stdStringToVariant(folder->getId()), ProjectTreeRole::ITEM_ID_ROLE);
        parent->appendRow(folderItem);
        itemMap_[folder->getId()] = folderItem;
        populateFolder(folderItem, folder, type);
        logger_->info(
            "Successfully appended project tree folder with ID: {}, name: {}, children: {}",
            folder->getId(),
            folder->getName(),
            folder->getChildren().size()
        );
    }

    void ProjectTreeModel::onProjectClosed() {
        logger_->info("Project closed, clearing model");
        clearModel();
    }

    void ProjectTreeModel::onDiagramAdded(const dp::DiagramMetadata *diagram) {
        auto parentId = diagram->getParent()->getId();
        if (itemMap_.contains(parentId)) {
            appendItem(itemMap_[parentId], diagram, TreeItemTypes::TreeItemType::DIAGRAM_FILE);
            emit itemReadyForEditing(itemMap_[diagram->getId()]->index());
        }
    }

    void ProjectTreeModel::onDiagramFolderAdded(const dp::NodeContainer *folder) {
        auto parentId = folder->getParent()->getId();
        if (itemMap_.contains(parentId)) {
            appendFolder(itemMap_[parentId], folder, TreeItemTypes::TreeItemType::DIAGRAM_FOLDER);
            emit itemReadyForEditing(itemMap_[folder->getId()]->index());
        }
    }

    void ProjectTreeModel::onDiagramRemoved(const std::string &diagramId) {
        if (!itemMap_.contains(diagramId)) return;

        const auto *diagramItem = itemMap_[diagramId];
        diagramItem->parent()->removeRow(diagramItem->row());
    }

    void ProjectTreeModel::onDiagramFolderRemoved(const std::string &folderId) {
        if (!itemMap_.contains(folderId)) return;

        const auto *folderItem = itemMap_[folderId];
        folderItem->parent()->removeRow(folderItem->row());
    }

    void ProjectTreeModel::onDiagramRenamed(
        const std::string &diagramId,
        const std::string &newName
    ) {
        if (!itemMap_.contains(diagramId)) return;

        auto *diagramItem = itemMap_[diagramId];
        diagramItem->setText(QString::fromStdString(newName));
    }

    void ProjectTreeModel::onDiagramFolderRenamed(
        const std::string &folderId,
        const std::string &newName
    ) {
        if (!itemMap_.contains(folderId)) return;

        auto *folderItem = itemMap_[folderId];
        folderItem->setText(QString::fromStdString(newName));
    }

    QIcon ProjectTreeModel::getIconForType(const TreeItemTypes::TreeItemType type) {
        switch (type) {
            case TreeItemTypes::TreeItemType::PROJECT_ROOT:
                return QIcon(":/icons/project.svg");
            case TreeItemTypes::TreeItemType::DIAGRAM_ROOT_FOLDER:
            case TreeItemTypes::TreeItemType::DIAGRAM_FOLDER:
                return QIcon(":/icons/folder.svg");
            case TreeItemTypes::TreeItemType::DIAGRAM_FILE:
                return QIcon(":/icons/diagram_file.svg");
            case TreeItemTypes::TreeItemType::ADD_DIAGRAM_ACTION_ITEM:
                return QIcon(":/icons/diagram_file_add.svg");
            default:
                return {};
        }
    }

    QVariant ProjectTreeModel::stdStringToVariant(const std::string &str) {
        return QVariant::fromValue(QString::fromStdString(str));
    }
}
