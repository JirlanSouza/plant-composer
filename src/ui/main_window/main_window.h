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
#include <memory>

#include "app_layout_manager.h"
#include "ui/project/project_view_manager.h"
#include "components_library/libraries_view_manager.h"
#include "diagram_editor/diagram_editor_manager.h"
#include "domain/diagram/component_instance_factory.h"
#include "domain/project/model/project.h"

namespace uil = ui::layout;
namespace uip = ui::project;
namespace ucl = ui::components_library;
namespace ude = ui::diagram_editor;
namespace dst = domain::settings;
namespace dcl = domain::components_library;
namespace dd = domain::diagram;
namespace dp = domain::project;

namespace ui::main_window {
    class AppMainWindow final : public QMainWindow {
        Q_OBJECT

    public:
        AppMainWindow(
            std::vector<dcl::Library> *libraries,
            dst::AppSettings *appSettings,
            dd::ComponentInstanceFactory *componentInstanceFactory,
            QWidget *parent
        );

        ~AppMainWindow() override;

    private:
        std::shared_ptr<dst::AppSettings> appSettings_;
        uil::AppLayoutManager *appLayoutManager_;
        uip::ProjectViewManager *projectViewManager_;
        ucl::LibrariesViewManager *librariesViewManager_;
        ude::DiagramManager *diagramManager_;
        ude::DiagramEditorManager *diagramEditorManager_;
        dd::ComponentInstanceFactory *componentInstanceFactory_;

    private slots:
        void showComponentLibraryPropertiesDialog(int libraryId, int componentId);
    };
}
