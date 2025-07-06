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

#include "domain/components_library/model/component_type.h"
#include "domain/components_library/model/data_type.h"
#include "domain/components_library/model/library.h"

namespace dcl = domain::components_library;

class TestComponentsLibrary : public QObject {
    Q_OBJECT

private slots:
    void testDataTypeToString() {
        QCOMPARE(dcl::dataTypeToString(dcl::DataType::REAL), std::string("REAL"));
        QCOMPARE(dcl::dataTypeToString(dcl::DataType::INTEGER), std::string("INTEGER"));
        QCOMPARE(dcl::dataTypeToString(dcl::DataType::BOOLEAN), std::string("BOOLEAN"));
        QCOMPARE(dcl::dataTypeToString(dcl::DataType::STRING), std::string("STRING"));
    }

    void testPortTypeCreation() {
        dcl::Measurement measurement1 = {"Pressure", dcl::DataType::REAL, "bar"};
        dcl::Measurement measurement2 = {"Flow", dcl::DataType::REAL, "l/h"};
        dcl::PortType portType = {1, "LiquidPort", {measurement1, measurement2}};

        QCOMPARE(portType.id, 1);
        QCOMPARE(portType.name, std::string("LiquidPort"));
        QCOMPARE(portType.measurements.size(), 2);
        QCOMPARE(portType.measurements[0].name, std::string("Pressure"));
        QCOMPARE(portType.measurements[1].unit, std::string("l/h"));
    }

    void testPortCreation() {
        dcl::Port port = {"liquidIn", 1};

        QCOMPARE(port.name, std::string("liquidIn"));
        QCOMPARE(port.typeId, 1);
    }

    void testSignalCreation() {
        dcl::Signal signal = {"on", dcl::DataType::BOOLEAN};

        QCOMPARE(signal.name, std::string("on"));
        QCOMPARE(signal.dataType, dcl::DataType::BOOLEAN);
    }

    void testConstantCreation() {
        dcl::Constant constant = {"diameter", dcl::DataType::REAL, "0.5"};

        QCOMPARE(constant.name, std::string("diameter"));
        QCOMPARE(constant.dataType, dcl::DataType::REAL);
        QCOMPARE(constant.defaultValue, std::string("0.5"));
    }

    void testComponentTypeCreation() {
        dcl::Constant constant = {"diameter", dcl::DataType::REAL, "0.5"};
        dcl::Port inputPort = {"liquidIn", 1};
        dcl::Port outputPort = {"liquidOut", 1};
        dcl::Signal inputSignal = {"open", dcl::DataType::BOOLEAN};

        dcl::ComponentType componentType = {
            "compType1",
            "Valve On/Off",
            "svgs/valve.svg",
            "/fmus/valve.fmu",
            {constant},
            {{inputPort}, {outputPort}},
            {{inputSignal}, {}}
        };

        QCOMPARE(componentType.id, std::string("compType1"));
        QCOMPARE(componentType.name, std::string("Valve On/Off"));
        QCOMPARE(componentType.symbolPath, std::string("svgs/valve.svg"));
        QCOMPARE(componentType.fmuPath, std::string("/fmus/valve.fmu"));
        QCOMPARE(componentType.constants.size(), 1);
        QCOMPARE(componentType.constants[0].name, std::string("diameter"));
        QCOMPARE(componentType.ports.inputs.size(), 1);
        QCOMPARE(componentType.ports.inputs[0].name, std::string("liquidIn"));
        QCOMPARE(componentType.ports.outputs.size(), 1);
        QCOMPARE(componentType.ports.outputs[0].name, std::string("liquidOut"));
        QCOMPARE(componentType.signalsIo.inputs.size(), 1);
        QCOMPARE(componentType.signalsIo.inputs[0].name, std::string("open"));
        QCOMPARE(componentType.signalsIo.outputs.empty(), true);
    }

    void testLibraryCreation() {
        dcl::PortType portType = {1, "LiquidPort", {}};
        dcl::ComponentType componentType = {
            "compType1",
            "Valve",
            "",
            "",
            {},
            {{}, {}},
            {{}, {}}
        };

        dcl::Library library = {
            "lib1",
            "Standard Library",
            "1.0.0",
            "Author",
            "A test library description",
            {portType},
            {componentType}
        };

        QCOMPARE(library.id, std::string("lib1"));
        QCOMPARE(library.name, std::string("Standard Library"));
        QCOMPARE(library.version, std::string("1.0.0"));
        QCOMPARE(library.author, std::string("Author"));
        QCOMPARE(library.description, std::string("A test library description"));
        QCOMPARE(library.portTypes.size(), 1);
        QCOMPARE(library.portTypes[0].name, std::string("LiquidPort"));
        QCOMPARE(library.components.size(), 1);
        QCOMPARE(library.components[0].name, std::string("Valve"));
    }
};
