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

#include "domain/common/ilogger_factory.h"
#include "domain/common/iuser_notifier.h"
#include "domain/common/id_factory.h"
#include "domain/settings/app_settings.h"
#include "domain/components_library/components_library_loader.h"
#include "domain/diagram/component_instance_factory.h"
#include "ui/main_window/app_main_windows.h"

namespace application {
    class PlantComposerApplication {
    public:
        const QString ORGANIZATION_NAME = "Plant Composer";
        const QString APPLICATION_NAME = "Plant Composer";

        PlantComposerApplication();

        void initialize(int argc, char *argv[]);

        [[nodiscard]] AppMainWindow *getMainWindow() const;

    private:
        std::unique_ptr<common::ILoggerFactory> loggerFactory_;
        std::unique_ptr<common::IUserNotifier> notifier_;
        std::unique_ptr<common::IDFactory> idFactory_;
        std::unique_ptr<settings::AppSettings> appSettings_;
        std::unique_ptr<components_library::ComponentsLibraryLoader> librariesLoader_;
        std::vector<components_library::Library> libraries_;
        std::unique_ptr<project::IProjectLoader> projectLoader_;
        std::unique_ptr<diagram::ComponentInstanceFactory> componentInstanceFactory_;
        std::unique_ptr<AppMainWindow> appMainWindow_;

        static std::string getAssetsDir();

        static void setupTranslations();
    };
}
