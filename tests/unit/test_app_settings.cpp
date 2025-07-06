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

#include "gtest/gtest.h"
#include <QCoreApplication>
#include <QSettings>
#include <QTemporaryFile>

#include "adapters/settings/qt_app_settings.h"

namespace as = adapters::settings;

class AppSettingsTest : public ::testing::Test {
protected:
    void SetUp() override {
        int argc = 0;
        static QCoreApplication app(argc, nullptr);

        ASSERT_TRUE(settingsFile.open());
        settingsFile.close();

        settings = new QSettings(settingsFile.fileName(), QSettings::IniFormat);
        settings->setValue("assets_dir", "/test/assets");
    }

    void TearDown() override {
        delete settings;
        QFile::remove(settingsFile.fileName());
    }

    QTemporaryFile settingsFile;
    QSettings *settings;
};

TEST_F(AppSettingsTest, GetComponentsLibraryDir) {
    as::QtAppSettings appSettings(settings);
    std::string expectedDir = "/test/assets/libraries";
    ASSERT_EQ(appSettings.getComponentsLibraryDir(), expectedDir);
}

TEST_F(AppSettingsTest, GetComponentIconPath) {
    as::QtAppSettings appSettings(settings);
    std::string expectedPath = "/test/assets/icons/puzzle.svg";
    ASSERT_EQ(appSettings.getComponentIconPath(), expectedPath);
}
