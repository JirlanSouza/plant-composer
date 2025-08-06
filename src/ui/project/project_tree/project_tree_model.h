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
#include <unordered_map>

#include "project_node_item.h"
#include "domain/project/model/project.h"
#include "ui/project/project_view_model.h"

namespace project {
    class ProjectTreeModel final : public QStandardItemModel {
        Q_OBJECT

    public:
        explicit ProjectTreeModel(
            common::ILoggerFactory *loggerFactory,
            ProjectViewModel *projectViewModel,
            QObject *parent = nullptr
        );

        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

        [[nodiscard]] Qt::DropActions supportedDropActions() const override;

        [[nodiscard]] QStringList mimeTypes() const override;

        [[nodiscard]] QMimeData *mimeData(const QModelIndexList &indexes) const override;

        bool dropMimeData(
            const QMimeData *data,
            Qt::DropAction action,
            int row,
            int column,
            const QModelIndex &parent
        ) override;

        [[nodiscard]] std::optional<ProjectNodeItem *> itemFromIndex(const QModelIndex &index) const;

    signals:
        void itemReadyForEditing(const QModelIndex &index);

    private slots:
        void onProjectClosed();

        void onProjectNodeAdded(const project::ProjectNode *node, ProjectCategoryType categoryType);

        void onProjectNodeRemoved(const std::string &nodeId);

        void onProjectNodeCut(const project::ProjectNode *node);

        void onProjectNodeCopied(
            const std::string &copiedNodeId,
            const project::ProjectNode *copyNode,
            ProjectCategoryType categoryType
        );

        void onProjectNodeRenamed(const std::string &fileId, const std::string &newName);

    private:
        std::unique_ptr<common::Ilogger> logger_;
        ProjectViewModel *projectViewModel_;
        std::unordered_map<std::string, ProjectNodeItem *> itemMap_;

        void buildModel();

        void clearModel();
    };
}
