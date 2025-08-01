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

#include "domain/components_library/model/library.h"

namespace components_library {
    class ComponentsLibraryModel final : public QStandardItemModel {
    public:
        explicit ComponentsLibraryModel(
            QObject *parent,
            const std::vector<Library> &libraries,
            std::string componentIconPath
        );

        void addLibrary(int libraryIndex, const Library &library);

        [[nodiscard]] QStringList mimeTypes() const override;

        [[nodiscard]] QMimeData *mimeData(const QModelIndexList &indexes) const override;

    private:
        const std::vector<Library> &libraries_;
        const std::string componentIconPath_;

        [[nodiscard]] QIcon newComponentIcon() const {
            return QIcon(QString::fromStdString(componentIconPath_));
        }
    };
}
