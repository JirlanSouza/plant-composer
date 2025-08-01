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

#include <QObject>

#include "domain/project/model/project.h"

namespace project {
    class TreeItemTypes : public QObject {
        Q_OBJECT

    public:
        enum TreeItemType {
            PROJECT_ROOT,
            DIAGRAM_ROOT_FOLDER,
            DIAGRAM_FOLDER,
            DIAGRAM_FILE,
            ADD_DIAGRAM_ACTION_ITEM,
        };

        Q_ENUM(TreeItemType)

        static TreeItemTypes::TreeItemType fromProjectCategoryAndNodeType(
            const ProjectCategoryType category,
            const NodeType nodeType
        ) {
            if (category == ProjectCategoryType::DIAGRAM) {
                if (nodeType == NodeType::FOLDER) {
                    return TreeItemTypes::DIAGRAM_FOLDER;
                } else if (nodeType == NodeType::FILE) {
                    return TreeItemTypes::DIAGRAM_FILE;
                }
            }
            return TreeItemTypes::PROJECT_ROOT;
        }

        static std::optional<ProjectCategoryType> toProjectCategory(const TreeItemType type) {
            switch (type) {
                case TreeItemType::DIAGRAM_ROOT_FOLDER:
                case TreeItemTypes::DIAGRAM_FOLDER:
                case TreeItemTypes::DIAGRAM_FILE:
                    return ProjectCategoryType::DIAGRAM;
                default:
                    return std::nullopt;
            }
        }

        static std::optional<NodeType> toNodeType(const TreeItemType type) {
            switch (type) {
                case TreeItemType::DIAGRAM_ROOT_FOLDER:
                case TreeItemTypes::DIAGRAM_FOLDER:
                    return NodeType::FOLDER;
                case TreeItemType::DIAGRAM_FILE:
                    return NodeType::FILE;
                default:
                    return std::nullopt;
            }
        }
    };
}
