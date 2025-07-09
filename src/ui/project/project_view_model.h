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

namespace dp = domain::project;

namespace ui::project {
    class ProjectViewModel : public QObject {
        Q_OBJECT

    public:
        explicit ProjectViewModel(dp::Project *project, QObject *parent = nullptr);

        [[nodiscard]] dp::Project *getProject() const;

        [[nodiscard]] dp::Project::DiagramMetadata getDiagramMetadata(const std::string &diagramId) const;

    public slots:
        void addDiagram(const std::string &name);

    signals:
        void diagramAdded(const dp::Project::DiagramMetadata &metadata);

    private:
        dp::Project *project_;
    };
}
