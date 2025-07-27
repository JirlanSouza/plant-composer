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

#include "components_library_model.h"

#include <qiodevice.h>
#include <QMimeData>
#include <utility>

#include "mime_types.h"

namespace components_library {
    ComponentsLibraryModel::ComponentsLibraryModel(
        QObject *parent,
        const std::vector<Library> &libraries,
        std::string componentIconPath
    ) : QStandardItemModel(parent),
        libraries_(libraries),
        componentIconPath_(std::move(componentIconPath)) {
        for (int i = 0; i < libraries.size(); ++i) {
            addLibrary(i, libraries[i]);
        }
    };

    void ComponentsLibraryModel::addLibrary(const int libraryIndex, const Library &library) {
        setColumnCount(1);
        auto *libraryItem = new QStandardItem(
            QIcon(":/icons/components_library.svg"),
            QString::fromStdString(library.name)
        );
        libraryItem->setFlags(Qt::ItemIsEnabled);
        libraryItem->setData(libraryIndex, Qt::UserRole);

        appendRow({libraryItem});

        for (int i = 0; i < library.components.size(); ++i) {
            auto component = library.components[i];
            auto *componentItem = new QStandardItem(
                QIcon(":/icons/component_type.svg"),
                QString::fromStdString(component.name)
            );
            componentItem->setFlags(
                Qt::ItemIsSelectable |
                Qt::ItemIsEnabled |
                Qt::ItemIsDragEnabled
            );
            componentItem->setData(i, Qt::UserRole);
            libraryItem->appendRow({componentItem});
        }
    }

    QStringList ComponentsLibraryModel::mimeTypes() const {
        return QStringList() << MIME_TYPE_COMPONENT;
    }

    QMimeData *ComponentsLibraryModel::mimeData(const QModelIndexList &indexes) const {
        if (indexes.isEmpty()) return nullptr;

        const QModelIndex &index = indexes.first();

        int libraryIndex = index.parent().data(Qt::UserRole).toInt();
        int componentIndex = index.data(Qt::UserRole).toInt();

        QByteArray encodedData;
        QDataStream stream(&encodedData, QIODevice::WriteOnly);
        stream << libraryIndex << componentIndex;

        auto *mimeData = new QMimeData();
        mimeData->setData(MIME_TYPE_COMPONENT, encodedData);
        return mimeData;
    }
}
