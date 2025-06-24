#pragma once
#include <string>

#include "components_library/model/library.h"
#include "main_window/main_window.h"
#include "settings/app_settings.h"

using ui::main_window::AppMainWindow;
using domain::components_library::Library;
using domain::settings::AppSettings;

class AppInitializer {
public:
    static std::unique_ptr<AppMainWindow> initialize(int argc, char *argv[]);

private:
    static std::string getAssetsDir();

    static void setupTranslations();

    static std::shared_ptr<AppSettings> createAppSettings();

    static std::vector<Library> loadComponentsLibraries(AppSettings *appSettings);
};
