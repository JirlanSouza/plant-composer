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

#include "project_tree/project_tree_model.h"
#include "project_tree/project_tree_view.h"
#include "project_view_model.h"
#include "new_diagram_dialog.h"

namespace dp = domain::project;

namespace ui::project {
    class ProjectViewManager : public QObject {
        Q_OBJECT

    public:
        explicit ProjectViewManager(
            ProjectViewModel *projectViewModel,
            QWidget *parent = nullptr
        );

        ~ProjectViewManager() override;

        [[nodiscard]] QWidget *getView() const;

    private:
        ProjectViewModel *projectViewModel_;
        ProjectTreeModel *projectTreeModel_;
        ProjectTreeView *projectTreeView_;
        NewDiagramDialog *newDiagramDialog_;
    };
}
