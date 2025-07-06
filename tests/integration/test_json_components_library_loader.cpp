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
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>

#include "adapters/components_library/json_components_library_loader.h"
#include "domain/components_library/model/library.h"

namespace acl = adapters::components_library;
namespace dcl = domain::components_library;

class JsonComponentsLibraryLoaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        int argc = 0;
        static QCoreApplication app(argc, nullptr);

        tempDir = QDir::tempPath() + "/test_libraries";
        QDir().mkpath(tempDir);

        QFile standardFile(tempDir + "/standard.json");
        ASSERT_TRUE(standardFile.open(QIODevice::WriteOnly | QIODevice::Text));
        QJsonObject standardJson;
        standardJson["id"] = "std-lib-id";
        standardJson["name"] = "Standard Library";
        standardJson["version"] = "1.0.0";
        standardJson["author"] = "Test Author";
        standardJson["description"] = "A test standard library";

        QJsonArray standardComponentsArray;
        QJsonObject valveComponent;
        valveComponent["id"] = "valve-id";
        valveComponent["name"] = "Valve";
        valveComponent["symbolPath"] = "valve.svg";
        valveComponent["fmuPath"] = "valve.fmu";

        QJsonArray valveConstantsArray;
        QJsonObject valveConstant1;
        valveConstant1["name"] = "diameter";
        valveConstant1["dataType"] = "REAL";
        valveConstant1["defaultValue"] = "0.5";
        valveConstantsArray.append(valveConstant1);
        valveComponent["constants"] = valveConstantsArray;

        QJsonObject valvePortsObject;
        QJsonArray valveInputPortsArray;
        QJsonObject valveInputPort1;
        valveInputPort1["name"] = "liquidIn";
        valveInputPort1["typeId"] = 1;
        valveInputPortsArray.append(valveInputPort1);
        valvePortsObject["inputs"] = valveInputPortsArray;

        QJsonArray valveOutputPortsArray;
        QJsonObject valveOutputPort1;
        valveOutputPort1["name"] = "liquidOut";
        valveOutputPort1["typeId"] = 1;
        valveOutputPortsArray.append(valveOutputPort1);
        valvePortsObject["outputs"] = valveOutputPortsArray;
        valveComponent["ports"] = valvePortsObject;

        QJsonObject valveSignalsIoObject;
        QJsonArray valveInputSignalsArray;
        QJsonObject valveInputSignal1;
        valveInputSignal1["name"] = "open";
        valveInputSignal1["dataType"] = "BOOLEAN";
        valveInputSignalsArray.append(valveInputSignal1);
        valveSignalsIoObject["inputs"] = valveInputSignalsArray;
        valveSignalsIoObject["outputs"] = QJsonArray();
        valveComponent["signalsIo"] = valveSignalsIoObject;

        standardComponentsArray.append(valveComponent);

        standardJson["components"] = standardComponentsArray;
        standardJson["portTypes"] = QJsonArray();
        standardFile.write(QJsonDocument(standardJson).toJson());
        standardFile.close();

        QFile motionFile(tempDir + "/motion.json");
        ASSERT_TRUE(motionFile.open(QIODevice::WriteOnly | QIODevice::Text));
        QJsonObject motionJson;
        motionJson["id"] = "mot-lib-id";
        motionJson["name"] = "Motion Library";
        motionJson["version"] = "1.0.0";
        motionJson["author"] = "Test Author";
        motionJson["description"] = "A test motion library";

        QJsonArray motionComponentsArray;
        QJsonObject motorComponent;
        motorComponent["id"] = "motor-id";
        motorComponent["name"] = "Motor";
        motorComponent["symbolPath"] = "motor.svg";
        motorComponent["fmuPath"] = "motor.fmu";

        QJsonArray motorConstantsArray;
        QJsonObject motorConstant1;
        motorConstant1["name"] = "power";
        motorConstant1["dataType"] = "REAL";
        motorConstant1["defaultValue"] = "100.0";
        motorConstantsArray.append(motorConstant1);
        motorComponent["constants"] = motorConstantsArray;

        QJsonObject motorPortsObject;
        QJsonArray motorInputPortsArray;
        QJsonObject motorInputPort1;
        motorInputPort1["name"] = "energyIn";
        motorInputPort1["typeId"] = 2;
        motorInputPortsArray.append(motorInputPort1);
        motorPortsObject["inputs"] = motorInputPortsArray;
        motorPortsObject["outputs"] = QJsonArray();
        motorComponent["ports"] = motorPortsObject;

        QJsonObject motorSignalsIoObject;
        QJsonArray motorOutputSignalsArray;
        QJsonObject motorOutputSignal1;
        motorOutputSignal1["name"] = "status";
        motorOutputSignal1["dataType"] = "BOOLEAN";
        motorOutputSignalsArray.append(motorOutputSignal1);
        motorSignalsIoObject["outputs"] = motorOutputSignalsArray;
        motorSignalsIoObject["inputs"] = QJsonArray();
        motorComponent["signalsIo"] = motorSignalsIoObject;

        motionComponentsArray.append(motorComponent);

        motionJson["components"] = motionComponentsArray;
        motionJson["portTypes"] = QJsonArray();
        motionFile.write(QJsonDocument(motionJson).toJson());
        motionFile.close();
    }

    void TearDown() override {
        QDir(tempDir).removeRecursively();
    }

    QString tempDir;
};

TEST_F(JsonComponentsLibraryLoaderTest, LoadLibraries) {
    acl::JsonComponentsLibraryLoader loader(tempDir.toStdString());
    std::vector<dcl::Library> libraries = loader.loadLibraries();

    ASSERT_EQ(libraries.size(), 2);

    std::ranges::sort(
        libraries,
        [](const dcl::Library &a, const dcl::Library &b) {
            return a.name < b.name;
        }
    );

    ASSERT_EQ(libraries[0].id, std::string("mot-lib-id"));
    ASSERT_EQ(libraries[0].name, std::string("Motion Library"));
    ASSERT_EQ(libraries[0].version, std::string("1.0.0"));
    ASSERT_EQ(libraries[0].author, std::string("Test Author"));
    ASSERT_EQ(libraries[0].description, std::string("A test motion library"));

    ASSERT_EQ(libraries[1].id, std::string("std-lib-id"));
    ASSERT_EQ(libraries[1].name, std::string("Standard Library"));
    ASSERT_EQ(libraries[1].version, std::string("1.0.0"));
    ASSERT_EQ(libraries[1].author, std::string("Test Author"));
    ASSERT_EQ(libraries[1].description, std::string("A test standard library"));
}

TEST_F(JsonComponentsLibraryLoaderTest, LoadComponentsFromLibraries) {
    acl::JsonComponentsLibraryLoader loader(tempDir.toStdString());
    std::vector<dcl::Library> libraries = loader.loadLibraries();

    ASSERT_EQ(libraries.size(), 2);

    std::ranges::sort(
        libraries,
        [](const dcl::Library &a, const dcl::Library &b) {
            return a.name < b.name;
        }
    );

    ASSERT_EQ(libraries[0].components.size(), 1);
    const auto &motor = libraries[0].components[0];
    ASSERT_EQ(motor.id, std::string("motor-id"));
    ASSERT_EQ(motor.name, std::string("Motor"));
    ASSERT_EQ(motor.symbolPath, std::string("motor.svg"));
    ASSERT_EQ(motor.fmuPath, std::string("motor.fmu"));

    ASSERT_EQ(motor.constants.size(), 1);
    ASSERT_EQ(motor.constants[0].name, std::string("power"));
    ASSERT_EQ(motor.constants[0].dataType, dcl::DataType::REAL);
    ASSERT_EQ(motor.constants[0].defaultValue, std::string("100.0"));

    ASSERT_EQ(motor.ports.inputs.size(), 1);
    ASSERT_EQ(motor.ports.inputs[0].name, std::string("energyIn"));
    ASSERT_EQ(motor.ports.inputs[0].typeId, 2);
    ASSERT_TRUE(motor.ports.outputs.empty());

    ASSERT_TRUE(motor.signalsIo.inputs.empty());
    ASSERT_EQ(motor.signalsIo.outputs.size(), 1);
    ASSERT_EQ(motor.signalsIo.outputs[0].name, std::string("status"));
    ASSERT_EQ(motor.signalsIo.outputs[0].dataType, dcl::DataType::BOOLEAN);

    ASSERT_EQ(libraries[1].components.size(), 1);
    const auto &valve = libraries[1].components[0];
    ASSERT_EQ(valve.id, std::string("valve-id"));
    ASSERT_EQ(valve.name, std::string("Valve"));
    ASSERT_EQ(valve.symbolPath, std::string("valve.svg"));
    ASSERT_EQ(valve.fmuPath, std::string("valve.fmu"));

    ASSERT_EQ(valve.constants.size(), 1);
    ASSERT_EQ(valve.constants[0].name, std::string("diameter"));
    ASSERT_EQ(valve.constants[0].dataType, dcl::DataType::REAL);
    ASSERT_EQ(valve.constants[0].defaultValue, std::string("0.5"));

    ASSERT_EQ(valve.ports.inputs.size(), 1);
    ASSERT_EQ(valve.ports.inputs[0].name, std::string("liquidIn"));
    ASSERT_EQ(valve.ports.inputs[0].typeId, 1);
    ASSERT_EQ(valve.ports.outputs.size(), 1);
    ASSERT_EQ(valve.ports.outputs[0].name, std::string("liquidOut"));
    ASSERT_EQ(valve.ports.outputs[0].typeId, 1);

    ASSERT_EQ(valve.signalsIo.inputs.size(), 1);
    ASSERT_EQ(valve.signalsIo.inputs[0].name, std::string("open"));
    ASSERT_EQ(valve.signalsIo.inputs[0].dataType, dcl::DataType::BOOLEAN);
    ASSERT_TRUE(valve.signalsIo.outputs.empty());
}
