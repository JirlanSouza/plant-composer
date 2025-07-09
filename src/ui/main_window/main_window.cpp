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

#include "main_window.h"

#include <iostream>
#include <qboxlayout.h>
#include <QMdiSubWindow>

namespace ui::main_window {
    AppMainWindow::AppMainWindow(
        std::vector<dcl::Library> *libraries,
        dst::AppSettings *appSettings,
        dd::ComponentInstanceFactory *componentInstanceFactory,
        QWidget *parent
    ) : QMainWindow(parent),
        appSettings_(appSettings), appLayoutManager_(new uil::AppLayoutManager(this)),
        librariesViewManager_(new components_library::LibrariesViewManager(libraries, appSettings, this)),
        componentInstanceFactory_(componentInstanceFactory) {
        auto project = new domain::project::Project(
            "proj1",
            "Test Project",
            "This is a test project.",
            "Test Author",
            "1.0.0",
            "/test_project.fbs"
        );

        projectViewModel_ = new uip::ProjectViewModel(project, this);
        diagramManager_ = new ui::diagram_editor::DiagramManager(
            libraries,
            appSettings_,
            componentInstanceFactory_,
            projectViewModel_,
            this
        );

        projectViewManager_ = new uip::ProjectViewManager(projectViewModel_, this);
        diagramEditorManager_ = new ui::diagram_editor::DiagramEditorManager(diagramManager_, projectViewModel_, this);
        appLayoutManager_->addSideBarView(tr("Project"), projectViewManager_->getView(), uil::LEFT);
        appLayoutManager_->addSideBarView(tr("Libraries"), librariesViewManager_->getView(), uil::RIGHT);
        appLayoutManager_->setCentralWidget(diagramEditorManager_->getView());
    }

    AppMainWindow::~AppMainWindow() = default;

    void AppMainWindow::showComponentLibraryPropertiesDialog(const int libraryId, const int componentId) {
    }
}
