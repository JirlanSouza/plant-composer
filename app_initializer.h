#pragma once
#include <string>

#include "components_library/model/library.h"
#include "main_window/main_window.h"
#include "settings/app_settings.h"

class AppInitializer {
public:
    static std::unique_ptr<MainWindow> initialize(int argc, char *argv[]);

private:
    static std::string getAssetsDir();

    static void setupTranslations();

    static std::unique_ptr<AppSettings> createAppSettings();

    static std::vector<Library> loadComponentsLibraries(AppSettings *appSettings);
};
