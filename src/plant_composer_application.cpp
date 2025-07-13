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

PlantComposerApplication::PlantComposerApplication() {
    appSettings_ = std::make_unique<QtAppSettings>(new QSettings(ORGANIZATION_NAME, APPLICATION_NAME));
    appSettings_->setAssetsDir(getAssetsDir());
    librariesLoader_ = std::make_unique<JsonComponentsLibraryLoader>(appSettings_->getComponentsLibraryDir());
    idFactory_ = std::make_unique<adapters::QUuidIdFactory>();
    componentInstanceFactory_ = std::make_unique<ComponentInstanceFactory>(
        idFactory_.get(),
        &libraries_,
        appSettings_.get()
    );
};


void PlantComposerApplication::initialize(int argc, char *argv[]) {
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    try {
        setupTranslations();
        libraries_ = std::move(librariesLoader_->loadLibraries());
        appMainWindow_ = std::make_unique<AppMainWindow>(
            &libraries_,
            appSettings_.get(),
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
