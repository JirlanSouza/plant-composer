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

#include <optional>

#include "domain/project/model/project.h"

namespace project::tree {
    enum class ItemType {
        PROJECT_ROOT,
        DIAGRAM_ROOT_FOLDER,
        DIAGRAM_FOLDER,
        DIAGRAM_FILE,
        ADD_DIAGRAM_ACTION_ITEM,
    };

    ItemType from(const ProjectCategoryType category, const NodeType nodeType, bool isRoot = false);

    std::optional<ProjectCategoryType> toProjectCategory(ItemType type);

    std::optional<NodeType> toNodeType(ItemType type);

    bool isAddNewFileNodeType(ItemType type);

    ItemType getAddNewFileNodeType(const ProjectCategoryType category);
}
