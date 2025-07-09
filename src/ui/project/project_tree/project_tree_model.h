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

#include <QStandardItemModel>
#include "domain/project/model/project.h"
#include "ui/project/project_view_model.h"

namespace dp = domain::project;

namespace ui::project {
    class ProjectTreeModel final : public QStandardItemModel {
        Q_OBJECT

    public:
        explicit ProjectTreeModel(
            ProjectViewModel *projectViewModel,
            const std::string &projectItemIconPath,
            QObject *parent
        );

        [[nodiscard]] QStringList mimeTypes() const override;

        [[nodiscard]] QMimeData *mimeData(const QModelIndexList &indexes) const override;

        [[nodiscard]] bool isAddDiagramItem(const QModelIndex &index) const;

    private slots:
        void onDiagramAdded(const domain::project::DiagramMetadata &metadata) const;

    private:
        ProjectViewModel *projectViewModel_;
        const std::string projectItemIconPath_;
        QStandardItem *projectTreeRootItem_;
        QStandardItem *diagramsFolderItem_;
        QStandardItem *addDiagramItem_;

        void buildProjectRootModel();

        void addDiagramFolder();

        void appendDiagram(const dp::DiagramMetadata &metadata) const;

        [[nodiscard]] QIcon newDiagramItemIcon() const {
            return QIcon(QString::fromStdString(projectItemIconPath_));
        }

        [[nodiscard]] QIcon buildIcon() const {
            return QIcon(QString::fromStdString(projectItemIconPath_));
        }
    };
}
