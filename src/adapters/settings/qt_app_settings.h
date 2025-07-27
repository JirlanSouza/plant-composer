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

#pragma once

#include <QSettings>

#include "domain/settings/app_settings.h"

namespace settings {
    class QtAppSettings final : public AppSettings {
    public:
        explicit QtAppSettings(QSettings *qSettings): AppSettings(), settings_(qSettings) {
        };

        std::string getAssetsDir() override;

        void setAssetsDir(const std::string &assetsDir) override;

        std::string getComponentsLibraryDir() override;

        std::string getComponentIconPath() override;

    private:
        const std::string assets_dir_key = "assets_dir";
        const std::string librariesDir_ = "/libraries";
        const std::string componentsIconPath_ = "/icons/puzzle.svg";

        QSettings *settings_;
    };
}
