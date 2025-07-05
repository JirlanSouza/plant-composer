#include "plant_composer_application.h"

#include <iostream>
#include <QTranslator>


using adapters::settings::QtAppSettings;
using adapters::components_library::JsonComponentsLibraryLoader;

PlantComposerApplication::PlantComposerApplication() {
    appSettings_ = std::make_unique<QtAppSettings>(new QSettings(ORGANIZATION_NAME, APPLICATION_NAME));
    appSettings_->setAssetsDir(getAssetsDir());
    librariesLoader_ = std::make_unique<JsonComponentsLibraryLoader>(appSettings_->getComponentsLibraryDir());
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
