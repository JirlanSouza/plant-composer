#pragma once
#include <string>

#include "domain/settings/app_settings.h"
#include "domain/components_library/components_library_loader.h"
#include "adapters/components_library/json_components_library_loader.h"
#include "adapters/settings/qt_app_settings.h"
#include "ui/main_window/main_window.h"
#include "ui/diagram_editor/diagram_service.h"

using ui::main_window::AppMainWindow;
using domain::settings::AppSettings;
using domain::components_library::Library;
using adapters::settings::QtAppSettings;
using adapters::components_library::JsonComponentsLibraryLoader;
using ui::diagram_editor::DiagramService;

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

    static std::string getAssetsDir();

    static void setupTranslations();
};
