#pragma once
#include "components_library_model.h"
#include "mime_types.h"

#include <qiodevice.h>
#include <QMimeData>
#include <utility>

namespace ui::components_library {
    ComponentsLibraryModel::ComponentsLibraryModel(QObject *parent,
                                                   const std::vector<dcl::Library> &libraries,
                                                   std::string componentIconPath) : QStandardItemModel(parent),
        libraries_(libraries),
        componentIconPath_(std::move(componentIconPath)) {
        for (int i = 0; i < libraries.size(); ++i) {
            addLibrary(i, libraries[i]);
        }
    };

    void ComponentsLibraryModel::addLibrary(const int libraryIndex, const dcl::Library &library) {
        setHorizontalHeaderLabels(QStringList() << tr("Libraries"));
        setColumnCount(1);
        auto *libraryItem = new QStandardItem(QString::fromStdString(library.name));
        libraryItem->setFlags(Qt::ItemIsEnabled);
        libraryItem->setData(libraryIndex, Qt::UserRole);

        appendRow({libraryItem});

        for (int i = 0; i < library.components.size(); ++i) {
            auto component = library.components[i];
            auto *componentItem = new QStandardItem(newComponentIcon(),
                                                    QString::fromStdString(component.name));
            componentItem->setFlags(Qt::ItemIsSelectable |
                                    Qt::ItemIsEnabled |
                                    Qt::ItemIsDragEnabled);
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
