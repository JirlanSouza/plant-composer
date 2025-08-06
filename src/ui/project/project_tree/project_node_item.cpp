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
        nodeType_(tree::from(categoryType, node->getType())) {
        setIcon(getIconFor(nodeType_));
        setFlags(getFlagsFor(nodeType_));
        setEditable(node->canBeRenamed());

        itemMap[node_->getId()] = this;
        populateItem(itemMap);
    }

    ProjectNodeItem::ProjectNodeItem(
        const Project *project,
        std::unordered_map<std::string, ProjectNodeItem *> &itemMap
    )
        : QStandardItem(QString::fromStdString(project->getName())),
        nodeType_(tree::ItemType::PROJECT_ROOT) {
        setIcon(getIconFor(nodeType_));
        setFlags(getFlagsFor(nodeType_));
        setEditable(false);

        std::map<ProjectCategoryType, ProjectCategory *> categories = project->getCategories();
        for (auto [categoryType, category]: categories) {
            if (category) {
                appendRow(new ProjectNodeItem(category, categoryType, itemMap));
            }
        }
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
        nodeType_(tree::from(categoryType, category->getType(), true)) {
        setIcon(getIconFor(nodeType_));
        setFlags(getFlagsFor(nodeType_));
        setEditable(category->canBeRenamed());

        itemMap[category->getId()] = this;
        appendRow(
            new ProjectNodeItem(
                QString("Add New ").append(category->getName()),
                categoryType_
            )
        );
        populateItem(itemMap);
    }

    ProjectNodeItem::ProjectNodeItem(
        const QString &text,
        const ProjectCategoryType categoryType
    ): QStandardItem(text),
        node_(nullptr),
        categoryType_(categoryType),
        nodeType_(tree::getAddNewFileNodeType(categoryType)) {
        setIcon(getIconFor(nodeType_));
        setFlags(getFlagsFor(nodeType_));
        setEditable(false);
    }


    const ProjectNode *ProjectNodeItem::getNode() const {
        return node_;
    }

    tree::ItemType ProjectNodeItem::getType() const {
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
        if (tree::isAddNewFileNodeType(nodeType_)) {
            return true;
        }

        const auto otherProjectNodeItem = dynamic_cast<const ProjectNodeItem *>(&other);

        if (!otherProjectNodeItem) {
            return true;
        }

        if (tree::isAddNewFileNodeType(otherProjectNodeItem->nodeType_)) {
            return false;
        }

        if (!node_ && otherProjectNodeItem->node_) {
            return false;
        }

        if (otherProjectNodeItem->node_ && otherProjectNodeItem->node_->isFolder() && (
                !node_ || node_ && !node_->isFolder())) {
            return false;
        }

        if (node_ && node_->isFolder() && (!otherProjectNodeItem->node_ || otherProjectNodeItem->node_ && !
                                           otherProjectNodeItem->node_->isFolder())) {
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

    QIcon ProjectNodeItem::getIconFor(tree::ItemType type) {
        switch (type) {
            case tree::ItemType::PROJECT_ROOT:
                return QIcon(":/icons/project.svg");
            case tree::ItemType::DIAGRAM_ROOT_FOLDER:
            case tree::ItemType::DIAGRAM_FOLDER:
                return QIcon(":/icons/folder.svg");
            case tree::ItemType::DIAGRAM_FILE:
                return QIcon(":/icons/diagram_file.svg");
            case tree::ItemType::ADD_DIAGRAM_ACTION_ITEM:
                return QIcon(":/icons/diagram_file_add.svg");
            default:
                return {};
        }
    }

    Qt::ItemFlags ProjectNodeItem::getFlagsFor(tree::ItemType type) {
        switch (type) {
            case tree::ItemType::PROJECT_ROOT:
                return Qt::ItemIsEnabled;
            case tree::ItemType::DIAGRAM_ROOT_FOLDER:
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
            case tree::ItemType::DIAGRAM_FOLDER:
            case tree::ItemType::DIAGRAM_FILE:
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
            case tree::ItemType::ADD_DIAGRAM_ACTION_ITEM:
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
            default:
                return {};
        }
    }

    QVariant ProjectNodeItem::stdStringToVariant(const std::string &str) {
        return QVariant::fromValue(QString::fromStdString(str));
    }
}
