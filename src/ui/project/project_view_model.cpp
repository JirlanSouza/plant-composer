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

#include "project_view_model.h"

#include "id/id_utils.h"

namespace ui::project {
    ProjectViewModel::ProjectViewModel(dp::Project *project, QObject *parent) : QObject(parent), project_(project) {
    }

    dp::Project *ProjectViewModel::getProject() const {
        return project_;
    }

    dp::Project::DiagramMetadata ProjectViewModel::getDiagramMetadata(const std::string &diagramId) const {
        for (const auto &metadata: project_->getDiagramsMetadata()) {
            if (metadata.id == diagramId) {
                return metadata;
            }
        }
        // TODO: Handle error if diagram not found. For now, return empty metadata.
        return {};
    }

    void ProjectViewModel::addDiagram(const std::string &name) {
        dp::Project::DiagramMetadata metadata{
            newId(),
            name,
            "diagrams/" + name + ".fbs"
        };
        project_->addDiagramMetadata(metadata);
        emit diagramAdded(metadata);
    }
}
