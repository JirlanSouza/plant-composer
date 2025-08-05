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

#include "project_tree_types.h"

namespace project::tree {

    ItemType from(const ProjectCategoryType category, const NodeType nodeType, const bool isRoot) {
        if (category == ProjectCategoryType::DIAGRAM) {
            if (isRoot) {
                return ItemType::DIAGRAM_ROOT_FOLDER;
            } else if (nodeType == NodeType::FOLDER) {
                return ItemType::DIAGRAM_FOLDER;
            } else if (nodeType == NodeType::FILE) {
                return ItemType::DIAGRAM_FILE;
            }
        }
        return ItemType::PROJECT_ROOT;
    }

    std::optional<ProjectCategoryType> toProjectCategory(const ItemType type) {
        switch (type) {
            case ItemType::DIAGRAM_ROOT_FOLDER:
            case ItemType::DIAGRAM_FOLDER:
            case ItemType::DIAGRAM_FILE:
                return ProjectCategoryType::DIAGRAM;
            default:
                return std::nullopt;
        }
    }

    std::optional<NodeType> toNodeType(const ItemType type) {
        switch (type) {
            case ItemType::DIAGRAM_ROOT_FOLDER:
            case ItemType::DIAGRAM_FOLDER:
                return NodeType::FOLDER;
            case ItemType::DIAGRAM_FILE:
                return NodeType::FILE;
            default:
                return std::nullopt;
        }
    }

    bool isAddNewFileNodeType(const ItemType type) {
        return type == ItemType::ADD_DIAGRAM_ACTION_ITEM;
    }

    ItemType getAddNewFileNodeType(const ProjectCategoryType category) {
        if (category == ProjectCategoryType::DIAGRAM) {
            return ItemType::ADD_DIAGRAM_ACTION_ITEM;
        }

        return ItemType::PROJECT_ROOT;
    }

}
