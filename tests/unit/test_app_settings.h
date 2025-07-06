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

#include <QtTest/QTest>
#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <QTemporaryFile>

#include "adapters/settings/qt_app_settings.h"

namespace as = adapters::settings;

class TestAppSettings : public QObject {
    Q_OBJECT

public:
    explicit TestAppSettings(QObject *parent = nullptr) : QObject(parent) {
    }

private slots:
    void initTestCase() {
        int argc = 0;
        static QCoreApplication app(argc, nullptr);
    }

    void testGetComponentsLibraryDir() {
        QTemporaryFile settingsFile;
        QVERIFY(settingsFile.open());
        settingsFile.close();

        QSettings settings(settingsFile.fileName(), QSettings::IniFormat);
        settings.setValue("assets_dir", "/test/assets");

        as::QtAppSettings appSettings(&settings);
        std::string expectedDir = "/test/assets/libraries";
        QCOMPARE(appSettings.getComponentsLibraryDir(), expectedDir);
    }

    void testGetComponentIconPath() {
        QTemporaryFile settingsFile;
        QVERIFY(settingsFile.open());
        settingsFile.close();

        QSettings settings(settingsFile.fileName(), QSettings::IniFormat);
        settings.setValue("assets_dir", "/test/assets");

        as::QtAppSettings appSettings(&settings);
        std::string expectedPath = "/test/assets/icons/puzzle.svg";
        QCOMPARE(appSettings.getComponentIconPath(), expectedPath);
    }
};
