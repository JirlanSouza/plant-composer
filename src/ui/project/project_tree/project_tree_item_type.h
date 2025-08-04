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

#include "mime_types.h"
#include "domain/project/model/project.h"

namespace project {
    class ProjectTreeTypes : public QObject {
        Q_OBJECT

    public:
        enum ItemType {
            PROJECT_ROOT,
            DIAGRAM_ROOT_FOLDER,
            DIAGRAM_FOLDER,
            DIAGRAM_FILE,
            ADD_DIAGRAM_ACTION_ITEM,
        };

        Q_ENUM(ItemType)

        static ItemType fromProjectCategoryAndNodeType(
            const ProjectCategoryType category,
            const NodeType nodeType,
            const bool isCategoryRootNode = false
        ) {
            if (category == ProjectCategoryType::DIAGRAM) {
                if (isCategoryRootNode) {
                    return DIAGRAM_ROOT_FOLDER;
                } else if (nodeType == NodeType::FOLDER) {
                    return DIAGRAM_FOLDER;
                } else if (nodeType == NodeType::FILE) {
                    return DIAGRAM_FILE;
                }
            }
            return PROJECT_ROOT;
        }

        static ItemType getAddNewFileNodeType(const ProjectCategoryType category) {
            switch (category) {
                case ProjectCategoryType::DIAGRAM: return ADD_DIAGRAM_ACTION_ITEM;
                default: return {};
            }
        }

        static bool isAddNewFileNodeType(const ItemType type) {
            switch (type) {
                case ADD_DIAGRAM_ACTION_ITEM:
                    return true;
                default:
                    return false;
            }
        }

        static std::optional<ProjectCategoryType> toProjectCategory(const ItemType type) {
            switch (type) {
                case DIAGRAM_ROOT_FOLDER:
                case DIAGRAM_FOLDER:
                case DIAGRAM_FILE:
                    return ProjectCategoryType::DIAGRAM;
                default:
                    return std::nullopt;
            }
        }

        static std::optional<NodeType> toNodeType(const ItemType type) {
            switch (type) {
                case DIAGRAM_ROOT_FOLDER:
                case DIAGRAM_FOLDER:
                    return NodeType::FOLDER;
                case DIAGRAM_FILE:
                    return NodeType::FILE;
                default:
                    return std::nullopt;
            }
        }

        static std::optional<QString> toMimeType(const ItemType type) {
            switch (type) {
                case DIAGRAM_ROOT_FOLDER:
                    return MIME_TYPE_PROJECT_CATEGORY;
                case DIAGRAM_FOLDER:
                    return MIME_TYPE_PROJECT_FOLDER;
                case DIAGRAM_FILE:
                    return MIME_TYPE_PROJECT_FILE;
                default: return std::nullopt;
            }
        }

        static Qt::ItemFlags flags(const ItemType type) {
            switch (type) {
                case PROJECT_ROOT:
                    return Qt::ItemIsEnabled;
                case DIAGRAM_ROOT_FOLDER:
                    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
                case DIAGRAM_FOLDER:
                case DIAGRAM_FILE:
                    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
                case ADD_DIAGRAM_ACTION_ITEM:
                    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
                default:
                    return {};
            }
        }

        static QIcon getIcon(const ItemType type) {
            switch (type) {
                case PROJECT_ROOT:
                    return QIcon(":/icons/project.svg");
                case DIAGRAM_ROOT_FOLDER:
                case DIAGRAM_FOLDER:
                    return QIcon(":/icons/folder.svg");
                case DIAGRAM_FILE:
                    return QIcon(":/icons/diagram_file.svg");
                case ADD_DIAGRAM_ACTION_ITEM:
                    return QIcon(":/icons/diagram_file_add.svg");
                default:
                    return {};
            }
        }
    };
}
