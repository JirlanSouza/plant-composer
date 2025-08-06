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

#include "project_tree_types.h"
#include "domain/project/model/project_node.h"
#include "domain/project/model/project.h"

namespace project {
    class ProjectNodeItem : public QStandardItem {
    public:
        ProjectNodeItem(
            const ProjectNode *node,
            ProjectCategoryType categoryType,
            std::unordered_map<std::string, ProjectNodeItem *> &itemMap
        );

        ProjectNodeItem(const Project *project, std::unordered_map<std::string, ProjectNodeItem *> &itemMap);

        [[nodiscard]] const ProjectNode *getNode() const;

        [[nodiscard]] tree::ItemType getType() const;

        [[nodiscard]] QString getId() const;

        [[nodiscard]] QString getName() const;

        [[nodiscard]] std::optional<ProjectContext> getContext() const;

        [[nodiscard]] std::optional<ProjectNodeItem *> getParentItem() const;

        void appendItemAndSort(ProjectNodeItem *item);

        bool operator<(const QStandardItem &other) const override;

    private:
        static QIcon getIconFor(tree::ItemType type);

        static Qt::ItemFlags getFlagsFor(tree::ItemType type);

        const ProjectNode *node_;
        ProjectCategoryType categoryType_;
        tree::ItemType nodeType_;

        ProjectNodeItem(
            const ProjectCategory *category,
            ProjectCategoryType categoryType,
            std::unordered_map<std::string, ProjectNodeItem *> &itemMap
        );

        ProjectNodeItem(
            const QString &text,
            ProjectCategoryType categoryType
        );

        void populateItem(std::unordered_map<std::string, ProjectNodeItem *> &itemMap);

        static QVariant stdStringToVariant(const std::string &str);
    };
}
