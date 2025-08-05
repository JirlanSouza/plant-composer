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

#include <map>
#include <string>
#include <memory>

#include "project_node.h"

namespace project {
    enum class ProjectCategoryType {
        DIAGRAM
    };

    std::string toString(const ProjectCategoryType &type);

    struct ProjectContext {
        ProjectCategoryType category;
        std::string parentId;
        NodeType nodeType;
        std::string nodeId;
    };

    class Project {
    public:
        Project(
            const std::string &id,
            const std::string &name,
            const std::string &description,
            const std::string &author,
            const std::string &version,
            const std::string &path,
            std::unique_ptr<ProjectCategory> diagrams
        );

        Project(
            const std::string &id,
            const std::string &name,
            const std::string &description,
            const std::string &author,
            const std::string &path
        );

        ~Project();

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] std::string getDescription() const;

        [[nodiscard]] std::string getAuthor() const;

        [[nodiscard]] std::string getVersion() const;

        [[nodiscard]] std::string getPath() const;

        [[nodiscard]] std::string getCategoryPath(ProjectCategoryType category) const;

        std::map<ProjectCategoryType, ProjectCategory *> getCategories() const;

        [[nodiscard]] std::optional<ProjectCategory *> getCategory(ProjectCategoryType category) const;

        [[nodiscard]] std::optional<ProjectNode *> findNode(ProjectCategoryType category, const std::string &id) const;

        void addNode(ProjectCategoryType category, std::unique_ptr<ProjectNode> node) const;

    private:
        static const std::string PROJECT_START_VERSION;
        static const std::string DIAGRAMS_ROOT_ID;
        static const std::string DIAGRAMS_ROOT_NAME;
        static const std::string DIAGRAMS_FOLDER_NAME;

        std::string id_;
        std::string name_;
        std::string description_;
        std::string author_;
        std::string version_;
        std::string path_;
        std::unique_ptr<ProjectCategory> diagrams_;
    };
}
