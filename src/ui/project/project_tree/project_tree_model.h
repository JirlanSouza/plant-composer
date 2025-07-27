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

#include "project_tree_item_type.h"
#include "domain/project/model/project.h"
#include "ui/project/project_view_model.h"

namespace project {
    enum ProjectTreeRole {
        ITEM_TYPE_ROLE = Qt::UserRole + 1, ITEM_ID_ROLE,
    };

    class ProjectTreeModel final : public QStandardItemModel {
        Q_OBJECT

    public:
        explicit ProjectTreeModel(
            common::ILoggerFactory *loggerFactory,
            ProjectViewModel *projectViewModel,
            QObject *parent = nullptr
        );

        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    signals:
        void itemReadyForEditing(const QModelIndex &index);

    private slots:
        void onProjectClosed();

        void onDiagramAdded(const project::DiagramMetadata *diagram);

        void onDiagramFolderAdded(const project::NodeContainer *folder);

        void onDiagramRemoved(const std::string &diagramId);

        void onDiagramFolderRemoved(const std::string &folderId);

        void onDiagramRenamed(
            const std::string &diagramId,
            const std::string &newName
        );

        void onDiagramFolderRenamed(
            const std::string &folderId,
            const std::string &newName
        );

    private:
        std::unique_ptr<common::Ilogger> logger_;
        ProjectViewModel *projectViewModel_;
        std::unordered_map<std::string, QStandardItem *> itemMap_;

        void buildModel();

        void clearModel();

        void populateFolder(QStandardItem *parentItem, const NodeContainer *folder, TreeItemTypes::TreeItemType type);

        void appendItem(QStandardItem *parent, const FileNode *file, TreeItemTypes::TreeItemType type);

        void appendFolder(QStandardItem *parent, const NodeContainer *folder, TreeItemTypes::TreeItemType type);

        static QIcon getIconForType(TreeItemTypes::TreeItemType type);

        static QVariant stdStringToVariant(const std::string &str);
    };
}
