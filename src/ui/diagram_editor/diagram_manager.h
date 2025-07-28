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
#include <unordered_map>

#include "editor_widget/diagram_view_model.h"
#include "domain/components_library/model/library.h"
#include "domain/settings/app_settings.h"
#include "domain/diagram/component_instance_factory.h"
#include "ui/project/project_view_model.h"

namespace diagram {
    class DiagramManager final : public QObject {
        Q_OBJECT

    public:
        explicit DiagramManager(
            std::vector<components_library::Library> *libraries,
            settings::AppSettings *appSettings,
            ComponentInstanceFactory *componentInstanceFactory,
            project::ProjectViewModel *projectViewModel,
            QObject *parent
        );

        ~DiagramManager() override;

        DiagramViewModel *getDiagram(const std::string &diagramId);

    public slots:
        void openDiagram(const project::FileNode *diagramMetadata);

    signals:
        void diagramOpened(std::string diagramId);

    private:
        std::vector<components_library::Library> *libraries_;
        std::unordered_map<std::string, DiagramViewModel *> openedDiagrams_;
        settings::AppSettings *appSettings_;
        ComponentInstanceFactory *componentInstanceFactory_;
        project::ProjectViewModel *projectViewModel_;
    };
}
