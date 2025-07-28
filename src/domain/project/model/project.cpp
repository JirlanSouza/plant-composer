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

#include "project.h"

namespace project {
    std::string toString(const ProjectCategoryType &category) {
        switch (category) {
            case ProjectCategoryType::DIAGRAM: return "Diagram";
            default: return "Unknown";
        }
    }

    const std::string Project::PROJECT_START_VERSION = "1.0.0";
    const std::string Project::DIAGRAMS_ROOT_ID = "diagrams_root";
    const std::string Project::DIAGRAMS_ROOT_NAME = "Diagrams";
    const std::string Project::DIAGRAMS_FOLDER_NAME = "diagrams";

    Project::Project(
        const std::string &id,
        const std::string &name,
        const std::string &description,
        const std::string &author,
        const std::string &version,
        const std::string &path,
        std::unique_ptr<ProjectCategory> diagrams
    )
        : id_(id),
        name_(name),
        description_(description),
        author_(author),
        version_(version),
        path_(path) {
        diagrams_ = std::move(diagrams);
    }

    Project::Project(
        const std::string &id,
        const std::string &name,
        const std::string &description,
        const std::string &author,
        const std::string &path
    )
        : id_(id),
        name_(name),
        description_(description),
        author_(author),
        version_(PROJECT_START_VERSION),
        path_(path) {
        diagrams_ = std::make_unique<ProjectCategory>(
            DIAGRAMS_ROOT_ID,
            DIAGRAMS_ROOT_NAME,
            DIAGRAMS_FOLDER_NAME
        );
    }

    Project::~Project() = default;

    std::string Project::getId() const { return id_; }
    std::string Project::getName() const { return name_; }
    std::string Project::getDescription() const { return description_; }
    std::string Project::getAuthor() const { return author_; }
    std::string Project::getVersion() const { return version_; }
    std::string Project::getPath() const { return path_; }

    std::string Project::getCategoryPath(const ProjectCategoryType category) const {
        switch (category) {
            case ProjectCategoryType::DIAGRAM:
                return DIAGRAMS_FOLDER_NAME;

            default: return "";
        }
    }

    std::optional<ProjectCategory *> Project::getCategory(const ProjectCategoryType category) const {
        switch (category) {
            case ProjectCategoryType::DIAGRAM:
                return diagrams_.get();

            default: return std::nullopt;
        }
    }

    std::optional<ProjectNode *> Project::findNode(const ProjectCategoryType category, const std::string &id) const {
        switch (category) {
            case ProjectCategoryType::DIAGRAM: {
                return diagrams_->findNode(id);
            }
            default: {
                return std::nullopt;
            }
        }
    }

    void Project::addNode(const ProjectCategoryType category, std::unique_ptr<ProjectNode> node) const {
        switch (category) {
            case ProjectCategoryType::DIAGRAM:
                diagrams_->addChild(std::move(node));
                break;
            default: {
            }
        }
    }
}
