#include "app_initializer.h"

#include <iostream>
#include <qcoreapplication.h>
#include <QLocale>
#include <QTranslator>

#include "components_library/json_components_library_loader.h"
#include "settings/qt_app_settings.h"

std::unique_ptr<MainWindow> AppInitializer::initialize(int argc, char *argv[]) {
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    setupTranslations();
    try {
        const std::string assetsDir = getAssetsDir();
        const auto appSettings = createAppSettings();
        appSettings->setAssetsDir(assetsDir);
        const std::vector<Library> libraries = loadComponentsLibraries(appSettings.get());
        return std::make_unique<MainWindow>(nullptr, std::move(libraries), appSettings.get());
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


void AppInitializer::setupTranslations() {
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


std::string AppInitializer::getAssetsDir() {
    auto assetsDir = std::string(SOURCE_ROOT) + "/assets";
    return assetsDir;
}

std::unique_ptr<AppSettings> AppInitializer::createAppSettings() {
    return std::make_unique<QtAppSettings>(new QSettings("Plant Composer", "PlantComposer"));
}

std::vector<Library> AppInitializer::loadComponentsLibraries(AppSettings *appSettings) {
    auto libraryLoader = JsonComponentsLibraryLoader(appSettings->getComponentsLibraryDir());
    return libraryLoader.loadLibraries();
}
