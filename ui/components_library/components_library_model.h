//
// Created by Jirlan Souza on 19/06/25.
//

#ifndef COMPONENTSLIBRARYMODEL_H
#define COMPONENTSLIBRARYMODEL_H

#include <QStandardItemModel>

#include "../../domain/components_library/model/library.h"


class ComponentsLibraryModel final : public QStandardItemModel {
public:
    explicit ComponentsLibraryModel(QObject *parent, const std::vector<Library> &libraries, std::string componentIconPath);

    void addLibrary(const Library &library);

private:
    const std::vector<Library> &libraries_;
    const std::string componentIconPath_;

    QIcon newComponentIcon() const {
        return QIcon(QString::fromStdString(componentIconPath_));
    }
};


#endif //COMPONENTSLIBRARYMODEL_H
