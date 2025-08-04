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

#include "project_node_item.h"

namespace project {
    ProjectNodeItem::ProjectNodeItem(
        const ProjectNode *node,
        const ProjectCategoryType categoryType,
        std::unordered_map<std::string, ProjectNodeItem *> &itemMap
    )
        : QStandardItem(QString::fromStdString(node->getName())),
        node_(node),
        categoryType_(categoryType),
        nodeType_(ProjectTreeTypes::fromProjectCategoryAndNodeType(categoryType, node->getType())) {
        setIcon(ProjectTreeTypes::getIcon(nodeType_));
        setFlags(ProjectTreeTypes::flags(nodeType_));
        setEditable(node->canBeRenamed());

        QStandardItem::setData(nodeType_, Role::ITEM_TYPE);
        QStandardItem::setData(stdStringToVariant(node->getId()), Role::ITEM_ID);

        itemMap[node_->getId()] = this;
        populateItem(itemMap);
    }

    ProjectNodeItem::ProjectNodeItem(
        const ProjectCategory *category,
        const ProjectCategoryType categoryType,
        std::unordered_map<std::string, ProjectNodeItem *> &itemMap
    ): QStandardItem(
            QString::fromStdString(category->getName())
        ),
        node_(category),
        categoryType_(categoryType),
        nodeType_(ProjectTreeTypes::fromProjectCategoryAndNodeType(categoryType, category->getType())) {
        setIcon(ProjectTreeTypes::getIcon(nodeType_));
        setFlags(ProjectTreeTypes::flags(nodeType_));
        setEditable(category->canBeRenamed());

        QStandardItem::setData(nodeType_, Role::ITEM_TYPE);
        QStandardItem::setData(stdStringToVariant(category->getId()), Role::ITEM_ID);

        itemMap[category->getId()] = this;
        auto *addNewFileNodeItem = new ProjectNodeItem(
            QString("Add New ").append(category->getName()),
            categoryType_,
            itemMap
        );

        appendRow(addNewFileNodeItem);
        populateItem(itemMap);
    }

    ProjectNodeItem::ProjectNodeItem(
        const QString &text,
        const ProjectCategoryType categoryType,
        std::unordered_map<std::string, ProjectNodeItem *> &itemMap
    ): QStandardItem(text),
        node_(nullptr),
        categoryType_(categoryType),
        nodeType_(ProjectTreeTypes::getAddNewFileNodeType(categoryType)) {
        setIcon(ProjectTreeTypes::getIcon(nodeType_));
        setFlags(ProjectTreeTypes::flags(nodeType_));
        setEditable(false);

        QStandardItem::setData(nodeType_, Role::ITEM_TYPE);
        QStandardItem::setData(stdStringToVariant("add_new_file"), Role::ITEM_ID);
    }


    const ProjectNode *ProjectNodeItem::getNode() const {
        return node_;
    }

    ProjectTreeTypes::ItemType ProjectNodeItem::getType() const {
        return nodeType_;
    }

    QString ProjectNodeItem::getId() const {
        return QString::fromStdString(node_->getId());
    }

    QString ProjectNodeItem::getName() const {
        return QString::fromStdString(node_->getName());
    }

    std::optional<ProjectContext> ProjectNodeItem::getContext() const {
        if (!node_) {
            return std::nullopt;
        }

        return ProjectContext{
            categoryType_,
            node_->getParent() ? node_->getParent()->getId() : "",
            node_->getType(),
            node_->getId()
        };
    }

    std::optional<ProjectNodeItem *> ProjectNodeItem::getParentItem() const {
        QStandardItem *parentItem = parent();
        if (!parentItem) {
            return std::nullopt;
        }
        return dynamic_cast<ProjectNodeItem *>(parentItem);
    }

    void ProjectNodeItem::appendItemAndSort(ProjectNodeItem *item) {
        if (!item) {
            return;
        }

        appendRow(item);
        sortChildren(0);
    }

    bool ProjectNodeItem::operator<(const QStandardItem &other) const {
        if (ProjectTreeTypes::isAddNewFileNodeType(nodeType_)) {
            return true;
        }

        const auto otherProjectNodeItem = dynamic_cast<const ProjectNodeItem *>(&other);

        if (!otherProjectNodeItem) {
            return true;
        }

        if (ProjectTreeTypes::isAddNewFileNodeType(otherProjectNodeItem->nodeType_)) {
            return false;
        }

        if (!node_ && otherProjectNodeItem->node_) {
            return false;
        }

        if (otherProjectNodeItem->node_ && otherProjectNodeItem->node_->isFolder() && (!node_ || node_ && !node_->isFolder())) {
            return false;
        }

        if (node_ && node_->isFolder() && (!otherProjectNodeItem->node_ || otherProjectNodeItem->node_ && !otherProjectNodeItem->node_->isFolder())) {
            return true;
        }

        return QStandardItem::operator<(other);
    }

    void ProjectNodeItem::populateItem(std::unordered_map<std::string, ProjectNodeItem *> &itemMap) {
        if (!node_ || !node_->isFolder()) {
            return;
        }

        const auto folderNode = dynamic_cast<const NodeContainer *>(node_);
        if (!folderNode) {
            return;
        }

        for (const auto &child: folderNode->getChildren()) {
            appendRow(new ProjectNodeItem(child, categoryType_, itemMap));
        }

        sortChildren(0);
    }

    QVariant ProjectNodeItem::stdStringToVariant(const std::string &str) {
        return QVariant::fromValue(QString::fromStdString(str));
    }
}
