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
        ProjectViewModel *projectViewModel,
        QObject *parent
    ) : QStandardItemModel(parent), projectViewModel_(projectViewModel) {
        buildModel();
        connect(projectViewModel_, &ProjectViewModel::diagramAdded, this, &ProjectTreeModel::onDiagramAdded);
        connect(
            projectViewModel_,
            &ProjectViewModel::diagramFolderAdded,
            this,
            &ProjectTreeModel::onDiagramFolderAdded
        );
    }

    void ProjectTreeModel::buildModel() {
        clear();
        itemMap_.clear();
        setColumnCount(1);

        auto *rootItem = new QStandardItem(
            getIconForType(TreeItemTypes::TreeItemType::PROJECT_ROOT),
            tr("Project: %1").arg(QString::fromStdString(projectViewModel_->getProject()->getName()))
        );
        rootItem->setFlags(Qt::ItemIsEnabled);
        rootItem->setData(TreeItemTypes::TreeItemType::PROJECT_ROOT, ProjectTreeRole::ITEM_TYPE_ROLE);
        appendRow(rootItem);

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

    void ProjectTreeModel::populateFolder(
        QStandardItem *parentItem,
        const dp::NodeContainer<dp::DiagramMetadata> *folder,
        const TreeItemTypes::TreeItemType type
    ) {
        for (const auto *item: folder->getChildren()) {
            if (item->isFile()) {
                appendItem(parentItem, dynamic_cast<const dp::DiagramMetadata *>(item), type);
            } else if (item->isFolder()) {
                appendFolder(parentItem, dynamic_cast<const dp::NodeContainer<dp::DiagramMetadata> *>(item), type);
            }
        }
    }

    void ProjectTreeModel::appendItem(
        QStandardItem *parent,
        const dp::DiagramMetadata *diagram,
        const TreeItemTypes::TreeItemType type
    ) {
        auto *diagramItem = new QStandardItem(
            getIconForType(type),
            QString::fromStdString(diagram->getName())
        );
        diagramItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
        diagramItem->setData(type, ProjectTreeRole::ITEM_TYPE_ROLE);
        diagramItem->setData(stdStringToVariant(diagram->getId()), ProjectTreeRole::ITEM_ID_ROLE);
        parent->appendRow(diagramItem);
        itemMap_[diagram->getId()] = diagramItem;
    }

    void ProjectTreeModel::appendFolder(
        QStandardItem *parent,
        const dp::NodeContainer<dp::DiagramMetadata> *folder,
        const TreeItemTypes::TreeItemType type
    ) {
        auto *folderItem = new QStandardItem(getIconForType(type), QString::fromStdString(folder->getName()));
        folderItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        folderItem->setData(type, ProjectTreeRole::ITEM_TYPE_ROLE);
        folderItem->setData(stdStringToVariant(folder->getId()), ProjectTreeRole::ITEM_ID_ROLE);
        parent->appendRow(folderItem);
        itemMap_[folder->getId()] = folderItem;
        populateFolder(folderItem, folder, type);
    }

    void ProjectTreeModel::onDiagramAdded(const dp::DiagramMetadata *diagram) {
        std::string parentId = diagram->getParent() ? diagram->getParent()->getId() : "";

        if (itemMap_.contains(parentId)) {
            appendItem(itemMap_[parentId], diagram, TreeItemTypes::TreeItemType::DIAGRAM_FILE);
        }
    }

    void ProjectTreeModel::onDiagramFolderAdded(const dp::NodeContainer<dp::DiagramMetadata> *folder) {
        std::string parentId = folder->getParent() ? folder->getParent()->getId() : "";

        if (itemMap_.contains(parentId)) {
            appendFolder(itemMap_[parentId], folder, TreeItemTypes::TreeItemType::DIAGRAM_FOLDER);
        }
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
