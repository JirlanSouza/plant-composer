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

#pragma once

#include <qstandarditemmodel.h>

#include "project_tree_item_type.h"
#include "domain/project/model/project_node.h"

namespace project {
    class ProjectNodeItem : public QStandardItem {
    public:
        enum Role {
            ITEM_TYPE = Qt::UserRole + 1, ITEM_ID,
        };

        ProjectNodeItem(
            const ProjectNode *node,
            ProjectCategoryType categoryType,
            std::unordered_map<std::string, ProjectNodeItem *> &itemMap
        );

        ProjectNodeItem(
            const ProjectCategory *category,
            ProjectCategoryType categoryType,
            std::unordered_map<std::string, ProjectNodeItem *> &itemMap
        );

        [[nodiscard]] const ProjectNode *getNode() const;

        [[nodiscard]] ProjectTreeTypes::ItemType getType() const;

        [[nodiscard]] QString getId() const;

        [[nodiscard]] QString getName() const;

        [[nodiscard]] std::optional<ProjectContext> getContext() const;

        [[nodiscard]] std::optional<ProjectNodeItem *> getParentItem() const;

        void appendItemAndSort(ProjectNodeItem *item);

        bool operator<(const QStandardItem &other) const override;

    private:
        const ProjectNode *node_;
        ProjectCategoryType categoryType_;
        ProjectTreeTypes::ItemType nodeType_;

        ProjectNodeItem(
            const QString &text,
            ProjectCategoryType categoryType,
            std::unordered_map<std::string, ProjectNodeItem *> &itemMap
        );

        void populateItem(std::unordered_map<std::string, ProjectNodeItem *> &itemMap);

        static QVariant stdStringToVariant(const std::string &str);
    };
}
