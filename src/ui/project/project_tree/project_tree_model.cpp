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

#include "mime_types.h"
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
            &ProjectViewModel::projectNodeMoved,
            this,
            &ProjectTreeModel::onProjectNodeCut
        );
        connect(
            projectViewModel_,
            &ProjectViewModel::projectNodeCopied,
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

        const auto type = index.data(ProjectNodeItem::Role::ITEM_TYPE).value<ProjectTreeTypes::ItemType>();
        const auto itemId = index.data(ProjectNodeItem::Role::ITEM_ID).toString().toStdString();
        const auto parentId = index.parent().data(ProjectNodeItem::Role::ITEM_ID).toString().toStdString();
        const auto newName = value.toString().toStdString();

        const auto categoryOpt = ProjectTreeTypes::toProjectCategory(type);
        const auto nodeTypeOpt = ProjectTreeTypes::toNodeType(type);

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

    QStringList ProjectTreeModel::mimeTypes() const {
        auto mimeTypes = QStringList() << MIME_TYPE_PROJECT_FOLDER << MIME_TYPE_PROJECT_FILE;
        logger_->debug("Getting supported mime types for project tree model: {}", mimeTypes.join(", ").toStdString());
        return mimeTypes;
    }

    QMimeData *ProjectTreeModel::mimeData(const QModelIndexList &indexes) const {
        if (indexes.isEmpty()) {
            logger_->warn("Empty indexes for mime data");
            return nullptr;
        }

        const QModelIndex index = indexes.first();
        const std::optional<ProjectNodeItem *> itemOpt = itemFromIndex(index);

        if (!itemOpt.has_value()) {
            logger_->warn("Invalid index for mime data: {}", index.row());
            return nullptr;
        }
        const ProjectNodeItem *item = itemOpt.value();
        const auto mimeTypeOpt = ProjectTreeTypes::toMimeType(item->getType());

        if (!mimeTypeOpt.has_value()) {
            logger_->warn("Tried to get mime data for unsupported item type: {}", static_cast<int>(item->getType()));
            return nullptr;
        }

        const auto id = item->getId();
        auto *mimeData = new QMimeData();
        mimeData->setData(mimeTypeOpt.value(), id.toUtf8());
        logger_->info(
            "Creating mime data for item type: {}, ID: {}",
            static_cast<int>(item->getType()),
            id.toStdString()
        );
        return mimeData;
    }

    bool ProjectTreeModel::dropMimeData(
        const QMimeData *data,
        Qt::DropAction action,
        int row,
        int column,
        const QModelIndex &parent
    ) {
        return false;
    }

    Qt::DropActions ProjectTreeModel::supportedDropActions() const {
        return Qt::MoveAction;
    }

    std::optional<ProjectNodeItem *> ProjectTreeModel::itemFromIndex(const QModelIndex &index) const {
        if (!index.isValid()) {
            logger_->warn("Invalid index provided to itemFromIndex");
            return std::nullopt;
        }

        QStandardItem *item = QStandardItemModel::itemFromIndex(index);

        if (!item) {
            logger_->warn("Null item provided to itemFromIndex");
            return std::nullopt;
        }


        auto nodeItem = dynamic_cast<ProjectNodeItem *>(item);

        if (!nodeItem) {
            logger_->warn("No casting possible to ProjectNodeItem in itemFromIndex");
            return std::nullopt;
        }

        return nodeItem;
    }

    void ProjectTreeModel::buildModel() {
        logger_->info("Building project tree model");
        logger_->info("Cleaning up previous model data");
        clearModel();
        setColumnCount(1);

        const auto project = projectViewModel_->getProject();

        if (!project) {
            logger_->warn("No project opened, skipping model build");
            return;
        }

        logger_->info("Creating project root item");
        auto *rootItem = new QStandardItem(
            ProjectTreeTypes::getIcon(ProjectTreeTypes::PROJECT_ROOT),
            tr("Project: %1").arg(QString::fromStdString(project->getName()))
        );
        rootItem->setFlags(ProjectTreeTypes::flags(ProjectTreeTypes::PROJECT_ROOT));
        rootItem->setData(ProjectTreeTypes::PROJECT_ROOT, ProjectNodeItem::Role::ITEM_TYPE);
        appendRow(rootItem);

        const std::optional<ProjectCategory *> diagramsRootOpt = project->getCategory(ProjectCategoryType::DIAGRAM);
        if (!diagramsRootOpt.has_value()) {
            logger_->warn("No diagrams root found in project, skipping diagram root item creation");
            return;
        }

        buildCategory(rootItem, diagramsRootOpt.value(), ProjectCategoryType::DIAGRAM);
    }

    void ProjectTreeModel::clearModel() {
        logger_->info("Clearing project tree model");
        clear();
        itemMap_.clear();
    }

    void ProjectTreeModel::buildCategory(
        QStandardItem *rootItem,
        const ProjectCategory *category,
        const ProjectCategoryType categoryType
    ) {
        logger_->info("Building {} root item", category->getName());
        auto *diagramsRootItem = new ProjectNodeItem(category, categoryType, itemMap_);
        rootItem->appendRow(diagramsRootItem);

        logger_->info("Successfully created {} root item", category->getName());
    }

    void ProjectTreeModel::onProjectClosed() {
        logger_->info("Project closed, clearing model");
        clearModel();
    }

    void ProjectTreeModel::onProjectNodeAdded(const ProjectNode *node, const ProjectCategoryType categoryType) {
        if (!node) {
            logger_->warn("Received null node in onProjectNodeAdded");
            return;
        }

        logger_->info("Adding node to project tree: {}", node->getId());
        auto parentId = node->getParent()->getId();

        if (!itemMap_.contains(parentId)) {
            logger_->warn("Invalid parent ID: {} for node with ID: {}", parentId, node->getId());
            return;
        }

        ProjectNodeItem *parentItem = itemMap_[parentId];
        parentItem->appendItemAndSort(new ProjectNodeItem(node, categoryType, itemMap_));
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

        const ProjectNodeItem *nodeItem = itemMap_[node->getId()];
        ProjectNodeItem *targetFolderItem = itemMap_[node->getParent()->getId()];

        if (nodeItem->parent() == targetFolderItem) {
            logger_->warn("Node is already in the target folder, no action taken");
            return;
        }

        QStandardItem *nodeParentItem = nodeItem->parent();
        if (!nodeParentItem) {
            logger_->warn("Node has invalid node parent in onProjectNodeCut");
            return;
        }

        auto *removedItem = dynamic_cast<ProjectNodeItem *>(nodeParentItem->takeChild(nodeItem->row()));

        if (!removedItem) {
            logger_->warn("No item found to cut for node ID: {}", node->getId());
            return;
        }

        removedItem->setText(QString::fromStdString(node->getName()));
        targetFolderItem->appendItemAndSort(removedItem);
        logger_->info(
            "Successfully cut node from project tree: {} to folder: {}",
            node->getId(),
            node->getParent()->getId()
        );
    }

    void ProjectTreeModel::onProjectNodeCopied(
        const std::string &copiedNodeId,
        const ProjectNode *copyNode,
        const ProjectCategoryType categoryType
    ) {
        if (!copyNode) {
            logger_->warn("Received null node in onProjectNodeCopied");
            return;
        }

        logger_->info("Coping node from project tree to folder: {}", copyNode->getParent()->getId());
        if (!itemMap_.contains(copiedNodeId) || !itemMap_.contains(copyNode->getParent()->getId())) {
            logger_->warn("Received invalid copied node id or target folder id in onProjectNodeCopied");
            return;
        }

        ProjectNodeItem *targetFolderItem = itemMap_[copyNode->getParent()->getId()];
        targetFolderItem->appendItemAndSort(new ProjectNodeItem(copyNode, categoryType, itemMap_));
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
        std::optional<ProjectNodeItem *> parentOpt = nodeItem->getParentItem();

        if (!parentOpt.has_value()) {
            logger_->warn("Node item has no parent in onProjectNodeRenamed");
            return;
        }

        parentOpt.value()->sortChildren(0);
    }
}
