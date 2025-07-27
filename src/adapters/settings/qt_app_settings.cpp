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

#include "qt_app_settings.h"

namespace settings {
    std::string QtAppSettings::getAssetsDir() {
        const QVariant value = settings_->value(assets_dir_key);
        if (!value.isValid() || value.typeId() != QMetaType::QString) {
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
