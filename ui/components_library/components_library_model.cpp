//
// Created by Jirlan Souza on 19/06/25.
//

#include "components_library_model.h"

#include <utility>

ComponentsLibraryModel::ComponentsLibraryModel(QObject *parent,
                                               const std::vector<Library> &libraries,
                                               std::string componentIconPath) : QStandardItemModel(parent),
    libraries_(libraries),
    componentIconPath_(std::move(componentIconPath)) {
    for (const auto &library: libraries) {
        addLibrary(library);
    }
};

void ComponentsLibraryModel::addLibrary(const Library &library) {
    setHorizontalHeaderLabels(QStringList() << tr("Libraries"));
    setColumnCount(1);
    auto *libraryItem = new QStandardItem(QString::fromStdString(library.name));
    libraryItem->setFlags(Qt::ItemIsEnabled);

    appendRow({libraryItem});

    for (auto component: library.components) {
        auto *componentItem = new QStandardItem(newComponentIcon(),
                                                QString::fromStdString(component.name));
        componentItem->setFlags(Qt::ItemIsSelectable |
                                Qt::ItemIsEnabled |
                                Qt::ItemIsDragEnabled);
        libraryItem->appendRow({componentItem});
    }
}
