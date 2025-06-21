#pragma once

#include "../../domain/settings/app_settings.h"
#include <QSettings>

class QtAppSettings final : public AppSettings {
public:
    explicit QtAppSettings(QSettings *qSettings): AppSettings(), settings_(qSettings) {
    };

    std::string getAssetsDir() override;

    void setAssetsDir(std::string assetsDir) override;

    std::string getComponentsLibraryDir() override;

    std::string getComponentIconPath() override;

private:
    const std::string assets_dir_key = "assets_dir";
    const std::string librariesDir_ = "/libraries";
    const std::string componentsIconPath_ = "/icons/puzzle.svg";

    QSettings *settings_;
};
