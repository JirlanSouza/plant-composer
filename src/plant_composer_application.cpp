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

#include "plant_composer_application.h"

#include <iostream>
#include <QTranslator>
#include <QSettings>
#include <QLocale>
#include <QCoreApplication>

#include "adapters/common/quuid_id_factory.h"
#include "adapters/common/spdlog_logger_factory.h"
#include "adapters/common/status_bar_notifier.h"
#include "adapters/components_library/json_components_library_loader.h"
#include "adapters/project/flat_buffer_project_loader.h"
#include "adapters/settings/qt_app_settings.h"

namespace application {
    PlantComposerApplication::PlantComposerApplication() {
        loggerFactory_ = std::make_unique<common::SpdlogLoggerFactory>();
        const auto logger = loggerFactory_->getLogger("Application");
        logger->info("Initializing Plant Composer Application...");
        idFactory_ = std::make_unique<common::QUuidIdFactory>();
        appSettings_ = std::make_unique<settings::QtAppSettings>(new QSettings(ORGANIZATION_NAME, APPLICATION_NAME));
        appSettings_->setAssetsDir(getAssetsDir());
        librariesLoader_ = std::make_unique<components_library::JsonComponentsLibraryLoader>(
            appSettings_->getComponentsLibraryDir()
        );
        projectLoader_ = std::make_unique<project::FlatBufferProjectLoader>(loggerFactory_.get(), idFactory_.get());
        componentInstanceFactory_ = std::make_unique<diagram::ComponentInstanceFactory>(
            idFactory_.get(),
            &libraries_,
            appSettings_.get()
        );
        logger->info("Plant Composer Application initialized.");
    };


    void PlantComposerApplication::initialize(int argc, char *argv[]) {
        Q_UNUSED(argc);
        Q_UNUSED(argv);

        try {
            setupTranslations();
            notifier_ = std::make_unique<common::StatusBarNotifier>();
            libraries_ = std::move(librariesLoader_->loadLibraries());
            appMainWindow_ = std::make_unique<AppMainWindow>(
                loggerFactory_.get(),
                dynamic_cast<common::StatusBarNotifier *>(notifier_.get()),
                &libraries_,
                appSettings_.get(),
                projectLoader_.get(),
                idFactory_.get(),
                componentInstanceFactory_.get(),
                nullptr
            );
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    AppMainWindow *PlantComposerApplication::getMainWindow() const {
        return appMainWindow_.get();
    }


    void PlantComposerApplication::setupTranslations() {
        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale: uiLanguages) {
            const QString baseName = "plant-composer_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                QCoreApplication::installTranslator(&translator);
                break;
            }
        }
    }


    std::string PlantComposerApplication::getAssetsDir() {
        auto assetsDir = std::string(SOURCE_ROOT) + "/assets";
        return assetsDir;
    }
}
