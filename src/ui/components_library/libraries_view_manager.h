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

#include "component_tree/components_library_model.h"
#include "component_tree/components_library_view.h"
#include "domain/settings/app_settings.h"

namespace components_library {
    class LibrariesViewManager : public QObject {
        Q_OBJECT

    public:
        explicit LibrariesViewManager(
            std::vector<Library> *libraries,
            settings::AppSettings *appSettings,
            QWidget *parent = nullptr
        );


        ~LibrariesViewManager() override;

        [[nodiscard]] QWidget *getView() const;

    private:
        ComponentsLibraryModel *libraryModel_;
        ComponentsLibraryView *libraryView_;
        std::vector<Library> *libraries_;
    };
}
