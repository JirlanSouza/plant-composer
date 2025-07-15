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

#include <QMimeData>
#include <QTreeView>
#include <QKeyEvent>

#include "project_tree_model.h"
#include "../project_view_model.h"

namespace ui::project {
    class ProjectTreeView final : public QTreeView {
        Q_OBJECT

    public:
        explicit ProjectTreeView(ProjectTreeModel *model, ProjectViewModel *viewModel, QWidget *parent = nullptr);

    signals:
        void componentPropertiesViewRequested(int libraryId, int componentId);

    private:
        ProjectTreeModel *model_;
        ProjectViewModel *viewModel_;

        void setupContextMenu();

    protected:
        void keyPressEvent(QKeyEvent *event) override;
    };
}
