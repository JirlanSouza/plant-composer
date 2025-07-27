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

#include <QMainWindow>
#include <QAction>

#include "app_layout_manager.h"
#include "ui/actions_manager/actions_manager.h"
#include "ui/project/project_view_manager.h"
#include "ui/project/project_view_model.h"
#include "components_library/libraries_view_manager.h"
#include "diagram_editor/diagram_editor_manager.h"
#include "domain/common/ilogger_factory.h"
#include "domain/diagram/component_instance_factory.h"
#include "domain/project/model/project.h"
#include "domain/project/project_loader.h"

namespace application {
    class AppMainWindow final : public QMainWindow {
        Q_OBJECT

    public:
        AppMainWindow(
            common::ILoggerFactory *loggerFactory,
            std::vector<components_library::Library> *libraries,
            settings::AppSettings *appSettings,
            project::IProjectLoader *projectLoader,
            common::IDFactory *idFactory,
            diagram::ComponentInstanceFactory *componentInstanceFactory,
            QWidget *parent
        );

        ~AppMainWindow() override;

    private:
        std::unique_ptr<common::Ilogger> logger_;
        settings::AppSettings *appSettings_;
        app_layout::AppLayoutManager *appLayoutManager_;
        app_actions::ActionsManager *actionsManager_;
        project::IProjectLoader *projectLoader_;
        project::ProjectViewManager *projectViewManager_;
        project::ProjectViewModel *projectViewModel_;
        components_library::LibrariesViewManager *librariesViewManager_;
        diagram::DiagramManager *diagramManager_;
        diagram::DiagramEditorManager *diagramEditorManager_;
        diagram::ComponentInstanceFactory *componentInstanceFactory_;

    private slots:
        void showComponentLibraryPropertiesDialog(int libraryId, int componentId);
    };
}
