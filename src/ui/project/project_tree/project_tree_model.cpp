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

namespace project {
    ProjectTreeModel::ProjectTreeModel(
        common::ILoggerFactory *loggerFactory,
        ProjectViewModel *projectViewModel,
        QObject *parent
    ) : QStandardItemModel(parent), logger_(loggerFactory->getLogger("ProjectTreeModel")),
        projectViewModel_(projectViewModel) {
        connect(projectViewModel_, &ProjectViewModel::projectOpened, this, &ProjectTreeModel::buildModel);
        connect(projectViewModel_, &ProjectViewModel::projectClosed, this, &ProjectTreeModel::onProjectClosed);
        connect(projectViewModel_, &ProjectViewModel::projectNodeAdded, this, &ProjectTreeModel::onProjectNodeAdded);
        connect(
            projectViewModel_,
            &ProjectViewModel::projectNodeRemoved,
            this,
            &ProjectTreeModel::onProjectNodeRemoved
        );
        connect(
            projectViewModel_,
            &ProjectViewModel::projectNodePastedAsCut,
            this,
            &ProjectTreeModel::onProjectNodeCut
        );
        connect(
            projectViewModel_,
            &ProjectViewModel::projectNodePastedAsCopy,
            this,
            &ProjectTreeModel::onProjectNodeCopied
        );
        connect(
            projectViewModel_,
            &ProjectViewModel::projectNodeRenamed,
            this,
            &ProjectTreeModel::onProjectNodeRenamed
        );
    }

    bool ProjectTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
        if (role != Qt::EditRole) {
            return QStandardItemModel::setData(index, value, role);
        }

        const auto type = index.data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();
        const auto itemId = index.data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
        const auto parentId = index.parent().data(ProjectTreeRole::ITEM_ID_ROLE).toString().toStdString();
        const auto newName = value.toString().toStdString();

        const auto categoryOpt = TreeItemTypes::toProjectCategory(type);
        const auto nodeTypeOpt = TreeItemTypes::toNodeType(type);

        if (!categoryOpt.has_value() || !nodeTypeOpt.has_value()) {
            logger_->warn(
                "Invalid item type for renaming: {}, ID: {}, parent ID: {}",
                static_cast<int>(type),
                itemId,
                parentId
            );
            return QStandardItemModel::setData(index, value, role);
        }

        projectViewModel_->renameProjectNode({categoryOpt.value(), parentId, nodeTypeOpt.value(), itemId}, newName);
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
        const auto diagramsRootOpt = projectViewModel_->getProject()->getCategory(ProjectCategoryType::DIAGRAM);

        if (!diagramsRootOpt.has_value()) {
            logger_->warn("No diagrams root found in project, skipping diagram root item creation");
            return;
        }

        const auto *diagramsRoot = diagramsRootOpt.value();
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
        const NodeContainer *folder,
        const TreeItemTypes::TreeItemType type
    ) {
        logger_->info(
            "Populating folder with ID: {}, name: {}, children: {}",
            folder->getId(),
            folder->getName(),
            folder->getChildren().size()
        );
        for (const auto *item: folder->getChildren()) {
            if (item->isFile()) {
                appendFileNode(parentItem, dynamic_cast<const FileNode *>(item), TreeItemTypes::DIAGRAM_FILE);
            } else if (item->isFolder()) {
                appendFolderNode(parentItem, dynamic_cast<const NodeContainer *>(item), type);
            }
        }
        parentItem->sortChildren(0, Qt::AscendingOrder);
        logger_->info(
            "Seccess populating folder with ID: {}, name: {}, children: {}",
            folder->getId(),
            folder->getName(),
            folder->getChildren().size()
        );
    }

    void ProjectTreeModel::appendFileNode(
        QStandardItem *parent,
        const FileNode *file,
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

    void ProjectTreeModel::appendFolderNode(
        QStandardItem *parent,
        const NodeContainer *folder,
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
        parent->sortChildren(0, Qt::AscendingOrder);
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

    void ProjectTreeModel::onProjectNodeAdded(const ProjectNode *node) {
        if (!node) {
            logger_->warn("Received null node in onProjectNodeAdded");
            return;
        }

        auto parentId = node->getParent()->getId();
        if (!itemMap_.contains(parentId)) {
            logger_->warn("Invalid parent ID: {} for node with ID: {}", parentId, node->getId());
            return;
        }

        if (node->isFile()) {
            appendFileNode(
                itemMap_[parentId],
                static_cast<const FileNode *>(node),
                TreeItemTypes::TreeItemType::DIAGRAM_FILE
            );
            itemMap_[parentId]->sortChildren(0, Qt::AscendingOrder);
        } else if (node->isFolder()) {
            appendFolderNode(
                itemMap_[parentId],
                static_cast<const NodeContainer *>(node),
                TreeItemTypes::TreeItemType::DIAGRAM_FOLDER
            );
        }

        emit itemReadyForEditing(itemMap_[node->getId()]->index());
    }

    void ProjectTreeModel::onProjectNodeRemoved(const std::string &nodeId) {
        logger_->info("Removing node from project tree: {}", nodeId);
        if (!itemMap_.contains(nodeId)) {
            logger_->warn("Received invalid node id in onProjectNodeRemoved");
            return;
        }

        const auto *nodeItem = itemMap_[nodeId];
        nodeItem->parent()->removeRow(nodeItem->row());
        itemMap_.erase(nodeId);
    }

    void ProjectTreeModel::onProjectNodeCut(const ProjectNode *node) {
        if (!node) {
            logger_->warn("Received null node in onProjectNodeCut");
            return;
        }

        logger_->info("Cutting node from project tree: {} to folder: {}", node->getId(), node->getParent()->getId());
        if (!itemMap_.contains(node->getId()) || !itemMap_.contains(node->getParent()->getId())) {
            logger_->warn("Received invalid node or target folder id in onProjectNodeCut");
            return;
        }

        QStandardItem *nodeItem = itemMap_[node->getId()];
        QStandardItem *targetFolderItem = itemMap_[node->getParent()->getId()];

        if (nodeItem->parent() == targetFolderItem) {
            logger_->warn("Node is already in the target folder, no action taken");
            return;
        }

        QStandardItem *nodeParentItem = nodeItem->parent();
        if (!nodeParentItem) {
            logger_->warn("Node has invalid node parent in onProjectNodeCut");
            return;
        }

        QStandardItem *removedItem = nodeParentItem->takeChild(nodeItem->row());

        if (!removedItem) {
            logger_->warn("No item found to cut for node ID: {}", node->getId());
            return;
        }

        targetFolderItem->appendRow(removedItem);
        targetFolderItem->sortChildren(0, Qt::AscendingOrder);
        logger_->info(
            "Successfully cut node from project tree: {} to folder: {}",
            node->getId(),
            node->getParent()->getId()
        );
    }

    void ProjectTreeModel::onProjectNodeCopied(const std::string& copiedNodeId, const ProjectNode *copyNode) {
        if (!copyNode) {
            logger_->warn("Received null node in onProjectNodeCopied");
            return;
        }

        logger_->info("Coping node from project tree to folder: {}", copyNode->getParent()->getId());
        if (!itemMap_.contains(copiedNodeId) || !itemMap_.contains(copyNode->getParent()->getId())) {
            logger_->warn("Received invalid copied node id or target folder id in onProjectNodeCopied");
            return;
        }

        QStandardItem *copiedNodeItem = itemMap_[copiedNodeId];
        QStandardItem *targetFolderItem = itemMap_[copyNode->getParent()->getId()];
        const auto itemType = copiedNodeItem->data(ProjectTreeRole::ITEM_TYPE_ROLE).value<TreeItemTypes::TreeItemType>();


        if (copyNode->isFile()) {
            appendFileNode(
                targetFolderItem,
                dynamic_cast<const FileNode *>(copyNode),
                itemType
            );
            targetFolderItem->sortChildren(0, Qt::AscendingOrder);
        } else if (copyNode->isFolder()) {
            appendFolderNode(
                targetFolderItem,
                dynamic_cast<const NodeContainer *>(copyNode),
                itemType
            );
        }

        logger_->info(
            "Successfully copy node from project tree: {} to folder: {}",
            copyNode->getId(),
            copyNode->getParent()->getId()
        );
    }

    void ProjectTreeModel::onProjectNodeRenamed(const std::string &fileId, const std::string &newName) {
        logger_->info("Renaming node in project tree ID: {} to {}", fileId, newName);
        if (!itemMap_.contains(fileId)) {
            logger_->warn("Received invalid node id in onProjectNodeRemoved");
            return;
        }

        auto *nodeItem = itemMap_[fileId];
        nodeItem->setText(QString::fromStdString(newName));
        nodeItem->parent()->sortChildren(0, Qt::AscendingOrder);
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
