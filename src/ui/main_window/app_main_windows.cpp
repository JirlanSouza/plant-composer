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

#include "application.h"

#include <qboxlayout.h>
#include <QMdiSubWindow>

namespace application {
    AppMainWindow::AppMainWindow(
        common::ILoggerFactory *loggerFactory,
        std::vector<components_library::Library> *libraries,
        settings::AppSettings *appSettings,
        project::IProjectLoader *projectLoader,
        common::IDFactory *idFactory,
        diagram::ComponentInstanceFactory *componentInstanceFactory,
        QWidget *parent
    ) : QMainWindow(parent),
        logger_(loggerFactory->getLogger("AppMainWindow")),
        appSettings_(appSettings),
        projectLoader_(projectLoader),
        actionsManager_(new app_actions::ActionsManager(this)),
        librariesViewManager_(new components_library::LibrariesViewManager(libraries, appSettings, this)),
        componentInstanceFactory_(componentInstanceFactory) {
        appLayoutManager_ = new app_layout::AppLayoutManager(this, actionsManager_);
        projectViewModel_ = new project::ProjectViewModel(loggerFactory, idFactory, projectLoader_, this);
        diagramManager_ = new diagram::DiagramManager(
            libraries,
            appSettings_,
            componentInstanceFactory_,
            projectViewModel_,
            this
        );

        projectViewManager_ = new project::ProjectViewManager(loggerFactory, projectViewModel_, actionsManager_, this);
        diagramEditorManager_ = new diagram::DiagramEditorManager(diagramManager_, projectViewModel_, this);

        appLayoutManager_->setupManuBar();
        appLayoutManager_->setupMainToolBar();

        appLayoutManager_->addSideBarView(tr("Project"), projectViewManager_->getView(), app_layout::LEFT);
        appLayoutManager_->addSideBarView(tr("Libraries"), librariesViewManager_->getView(), app_layout::RIGHT);
        appLayoutManager_->setCentralWidget(diagramEditorManager_->getView());
    }

    AppMainWindow::~AppMainWindow() = default;

    void AppMainWindow::showComponentLibraryPropertiesDialog(const int libraryId, const int componentId) {
    }
}
