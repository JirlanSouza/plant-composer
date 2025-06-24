#include "qt_app_settings.h"

#include <iostream>

namespace adapters::settings {
    std::string QtAppSettings::getAssetsDir() {
        const QVariant value = settings_->value(assets_dir_key);
        if (!value.isValid() || value.typeId() != QVariant::String) {
            return "";
        }

        return value.toString().toStdString();
    }

    void QtAppSettings::setAssetsDir(const std::string &assetsDir) {
        settings_->setValue(assets_dir_key, QVariant::fromValue(QString::fromStdString(assetsDir)));
    }


    std::string QtAppSettings::getComponentsLibraryDir() {
        const std::string assetsDir = getAssetsDir();
        return assetsDir + librariesDir_;
    }

    std::string QtAppSettings::getComponentIconPath() {
        const std::string assetsDir = getAssetsDir();
        return assetsDir + componentsIconPath_;
    }
}
