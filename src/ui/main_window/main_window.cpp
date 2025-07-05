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
#include <QLabel>

namespace ui::main_window {
    AppMainWindow::AppMainWindow(
        std::vector<dcl::Library> *libraries,
        dst::AppSettings *appSettings,
        QWidget *parent
    ) : QMainWindow(parent),
        appSettings_(appSettings), appLayoutManager_(new uil::AppLayoutManager(this)),
        librariesViewManager_(new components_library::LibrariesViewManager(libraries, appSettings, this)),
        diagramEditorManager_(
            new ui::diagram_editor::DiagramEditorManager(
                new ui::diagram_editor::DiagramService(libraries, appSettings, this)
            )
        ) {
        // Change type and namespace
        auto t0 = new QLabel("Project left side bar");
        auto l0 = new QVBoxLayout();
        l0->addWidget(t0);
        auto w0 = new QWidget(this);
        w0->setLayout(l0);

        appLayoutManager_->addSideBarView("Project", w0, uil::LEFT);
        auto t1 = new QLabel("Version control left side bar");
        auto l1 = new QVBoxLayout();
        l1->addWidget(t1);
        auto w1 = new QWidget(this);
        w1->setLayout(l1);
        appLayoutManager_->addSideBarView("Version control", w1, uil::LEFT);


        appLayoutManager_->addSideBarView("Libraries", librariesViewManager_->getView(), uil::RIGHT);
        auto t = new QLabel("Drivers content");
        auto l = new QVBoxLayout();
        l->addWidget(t);
        auto w = new QWidget(this);
        w->setLayout(l);
        appLayoutManager_->addSideBarView("Properties", w, uil::RIGHT);
        appLayoutManager_->setCentralWidget(diagramEditorManager_->getView());

        /*connect(libraryView_, &ucl::ComponentsLibraryView::componentPropertiesViewRequested, this,
                &AppMainWindow::showComponentLibraryPropertiesDialog);*/
    }

    AppMainWindow::~AppMainWindow() = default;

    void AppMainWindow::showComponentLibraryPropertiesDialog(const int libraryId, const int componentId) {
        // const dcl::Library library = libraries_->at(libraryId);
        // const dcl::Component component = library.components[componentId];
        // std::unordered_map<int, dcl::PortType> portTypes;
        //
        // for (const auto &portType: library.portTypes) {
        //     portTypes.emplace(portType.id, portType);
        // }
        //
        // const auto dialog = new ucl::LibraryComponentPropertiesView(this, component, portTypes);
        // dialog->exec();
    }
}
