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
#include <string>
#include <memory>

#include "domain/settings/app_settings.h"
#include "domain/components_library/components_library_loader.h"
#include "adapters/components_library/json_components_library_loader.h"
#include "adapters/settings/qt_app_settings.h"
#include "adapters/shared/quuid_id_factory.h"
#include "ui/main_window/main_window.h"
#include "ui/diagram_editor/diagram_manager.h"
#include "domain/diagram/component_instance_factory.h"

using ui::main_window::AppMainWindow;
using domain::settings::AppSettings;
using domain::components_library::Library;
using adapters::settings::QtAppSettings;
using adapters::components_library::JsonComponentsLibraryLoader;
using ui::diagram_editor::DiagramManager;
using domain::diagram::ComponentInstanceFactory;

class PlantComposerApplication {
public:
    const QString ORGANIZATION_NAME = "Plant Composer";
    const QString APPLICATION_NAME = "Plant Composer";

    PlantComposerApplication();

    void initialize(int argc, char *argv[]);

    [[nodiscard]] AppMainWindow *getMainWindow() const;

private:
    std::unique_ptr<QtAppSettings> appSettings_;
    std::unique_ptr<JsonComponentsLibraryLoader> librariesLoader_;
    std::vector<Library> libraries_;
    std::unique_ptr<AppMainWindow> appMainWindow_;
    std::unique_ptr<adapters::QUuidIdFactory> idFactory_;
    std::unique_ptr<ComponentInstanceFactory> componentInstanceFactory_;

    static std::string getAssetsDir();

    static void setupTranslations();
};
