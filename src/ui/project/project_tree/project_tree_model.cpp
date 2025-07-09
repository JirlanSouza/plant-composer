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

#include "project_tree_model.h"

#include <qiodevice.h>
#include <QMimeData>

#include "mime_types.h"

namespace ui::project {
    ProjectTreeModel::ProjectTreeModel(
        ProjectViewModel *projectViewModel,
        const std::string &projectItemIconPath,
        QObject *parent
    ) : QStandardItemModel(parent), projectViewModel_(projectViewModel), projectItemIconPath_(projectItemIconPath) {
        buildProjectRootModel();
        addDiagramFolder();

        connect(projectViewModel_, &ProjectViewModel::diagramAdded, this, &ProjectTreeModel::onDiagramAdded);
    }

    void ProjectTreeModel::buildProjectRootModel() {
        setColumnCount(1);
        projectTreeRootItem_ = new QStandardItem(
            QIcon(":/icons/project.svg"),
            tr("Project: %1").arg(QString::fromStdString(projectViewModel_->getProject()->getName()))
        );
        projectTreeRootItem_->setFlags(Qt::ItemIsEnabled);
        appendRow(projectTreeRootItem_);
    }


    void ProjectTreeModel::addDiagramFolder() {
        if (!projectTreeRootItem_) {
            return;
        }

        setColumnCount(1);
        diagramsFolderItem_ = new QStandardItem(QIcon(":/icons/folder.svg"), tr("Diagrams"));
        diagramsFolderItem_->setFlags(Qt::ItemIsEnabled);
        projectTreeRootItem_->appendRow({diagramsFolderItem_});

        addDiagramItem_ = new QStandardItem(
            QIcon(":/icons/diagram_file_add.svg"),
            tr("Add Diagram")
        );
        addDiagramItem_->setFlags(Qt::ItemIsEnabled);
        diagramsFolderItem_->appendRow({addDiagramItem_});

        const auto diagramsMetadata = projectViewModel_->getProject()->getDiagramsMetadata();
        for (const auto &diagramMetadata: diagramsMetadata) {
            appendDiagram(diagramMetadata);
        }
    }

    void ProjectTreeModel::appendDiagram(const dp::DiagramMetadata &metadata) const {
        auto *diagramItem = new QStandardItem(
            QIcon(":/icons/diagram_file.svg"),
            QString::fromStdString(metadata.name)
        );
        diagramItem->setFlags(
            Qt::ItemIsSelectable |
            Qt::ItemIsEnabled |
            Qt::ItemIsDragEnabled
        );
        diagramItem->setData(QVariant::fromValue(QString::fromStdString(metadata.id)), Qt::UserRole);
        diagramsFolderItem_->appendRow({diagramItem});
    }

    void ProjectTreeModel::onDiagramAdded(const dp::DiagramMetadata &metadata) const {
        appendDiagram(metadata);
    }

    QStringList ProjectTreeModel::mimeTypes() const {
        return QStringList() << MIME_TYPE_DIAGRAM;
    }

    QMimeData *ProjectTreeModel::mimeData(const QModelIndexList &indexes) const {
        if (indexes.isEmpty()) return nullptr;

        const QModelIndex &index = indexes.first();
        if (!index.isValid() || isAddDiagramItem(index) || !index.parent().isValid() || index.parent() !=
            diagramsFolderItem_->index()) {
            return nullptr;
        }

        const auto diagramId = index.data(Qt::UserRole).toString();

        QByteArray encodedData;
        QDataStream stream(&encodedData, QIODevice::WriteOnly);
        stream << diagramId;

        auto *mimeData = new QMimeData();
        mimeData->setData(MIME_TYPE_DIAGRAM, encodedData);
        return mimeData;
    }

    bool ProjectTreeModel::isAddDiagramItem(const QModelIndex &index) const {
        return index.isValid() && index == addDiagramItem_->index();
    }
}
