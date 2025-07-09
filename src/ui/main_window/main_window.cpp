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
        diagramManager_ = new ui::diagram_editor::DiagramManager(
            libraries,
            appSettings,
            componentInstanceFactory,
            this
        );
        diagramEditorManager_ = new ui::diagram_editor::DiagramEditorManager(diagramManager_);


        auto project = new domain::project::Project(
            "proj1",
            "Test Project",
            "This is a test project.",
            "Test Author",
            "1.0.0",
            "/test_project.fbs"
        );

        project->addDiagramMetadata({"diag1", "Diagram 1", "diagrams/diag1->fbs"});
        project->addDiagramMetadata({"diag2", "Diagram 2", "diagrams/diag2.fbs"});

        projectViewManager_ = new uip::ProjectViewManager(project, this);
        appLayoutManager_->addSideBarView("Project", projectViewManager_->getView(), uil::LEFT);
        appLayoutManager_->addSideBarView("Libraries", librariesViewManager_->getView(), uil::RIGHT);
        appLayoutManager_->setCentralWidget(diagramEditorManager_->getView());

        connect(
            projectViewManager_,
            &uip::ProjectViewManager::addDiagramClicked,
            diagramEditorManager_,
            &ui::diagram_editor::DiagramEditorManager::addNewDiagram
        );
    }

    AppMainWindow::~AppMainWindow() = default;

    void AppMainWindow::showComponentLibraryPropertiesDialog(const int libraryId, const int componentId) {
    }
}
