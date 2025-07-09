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

#include "project_view_manager.h"

namespace ui::project {
    ProjectViewManager::ProjectViewManager(ProjectViewModel *projectViewModel, QWidget *parent): QObject(parent),
        projectViewModel_(projectViewModel) {
        projectTreeModel_ = new ProjectTreeModel(projectViewModel_, "", this);
        projectTreeView_ = new ProjectTreeView(projectTreeModel_, parent);
        newDiagramDialog_ = new NewDiagramDialog(projectTreeView_);

        connect(
            projectTreeView_,
            &ProjectTreeView::doubleClicked,
            this,
            [this](const QModelIndex &index) {
                if (index.isValid() && projectTreeModel_->isAddDiagramItem(index)) {
                    newDiagramDialog_->exec();
                }
            }
        );

        connect(
            newDiagramDialog_,
            &NewDiagramDialog::diagramNameEntered,
            projectViewModel_,
            &ProjectViewModel::addDiagram
        );
    }

    ProjectViewManager::~ProjectViewManager() = default;

    QWidget *ProjectViewManager::getView() const {
        return projectTreeView_;
    }
}
