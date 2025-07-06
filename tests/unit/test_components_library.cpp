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

#include "domain/components_library/model/component_type.h"
#include "domain/components_library/model/data_type.h"
#include "domain/components_library/model/library.h"

namespace dcl = domain::components_library;

TEST(ComponentsLibraryTest, DataTypeToString) {
    ASSERT_EQ(dcl::dataTypeToString(dcl::DataType::REAL), std::string("REAL"));
    ASSERT_EQ(dcl::dataTypeToString(dcl::DataType::INTEGER), std::string("INTEGER"));
    ASSERT_EQ(dcl::dataTypeToString(dcl::DataType::BOOLEAN), std::string("BOOLEAN"));
    ASSERT_EQ(dcl::dataTypeToString(dcl::DataType::STRING), std::string("STRING"));
}

TEST(ComponentsLibraryTest, PortTypeCreation) {
    dcl::Measurement measurement1 = {"Pressure", dcl::DataType::REAL, "bar"};
    dcl::Measurement measurement2 = {"Flow", dcl::DataType::REAL, "l/h"};
    dcl::PortType portType = {1, "LiquidPort", {measurement1, measurement2}};

    ASSERT_EQ(portType.id, 1);
    ASSERT_EQ(portType.name, std::string("LiquidPort"));
    ASSERT_EQ(portType.measurements.size(), 2);
    ASSERT_EQ(portType.measurements[0].name, std::string("Pressure"));
    ASSERT_EQ(portType.measurements[1].unit, std::string("l/h"));
}

TEST(ComponentsLibraryTest, PortCreation) {
    dcl::Port port = {"liquidIn", 1};

    ASSERT_EQ(port.name, std::string("liquidIn"));
    ASSERT_EQ(port.typeId, 1);
}

TEST(ComponentsLibraryTest, SignalCreation) {
    dcl::Signal signal = {"on", dcl::DataType::BOOLEAN};

    ASSERT_EQ(signal.name, std::string("on"));
    ASSERT_EQ(signal.dataType, dcl::DataType::BOOLEAN);
}

TEST(ComponentsLibraryTest, ConstantCreation) {
    dcl::Constant constant = {"diameter", dcl::DataType::REAL, "0.5"};

    ASSERT_EQ(constant.name, std::string("diameter"));
    ASSERT_EQ(constant.dataType, dcl::DataType::REAL);
    ASSERT_EQ(constant.defaultValue, std::string("0.5"));
}

TEST(ComponentsLibraryTest, ComponentTypeCreation) {
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

    ASSERT_EQ(componentType.id, std::string("compType1"));
    ASSERT_EQ(componentType.name, std::string("Valve On/Off"));
    ASSERT_EQ(componentType.symbolPath, std::string("svgs/valve.svg"));
    ASSERT_EQ(componentType.fmuPath, std::string("/fmus/valve.fmu"));
    ASSERT_EQ(componentType.constants.size(), 1);
    ASSERT_EQ(componentType.constants[0].name, std::string("diameter"));
    ASSERT_EQ(componentType.ports.inputs.size(), 1);
    ASSERT_EQ(componentType.ports.inputs[0].name, std::string("liquidIn"));
    ASSERT_EQ(componentType.ports.outputs.size(), 1);
    ASSERT_EQ(componentType.ports.outputs[0].name, std::string("liquidOut"));
    ASSERT_EQ(componentType.signalsIo.inputs.size(), 1);
    ASSERT_EQ(componentType.signalsIo.inputs[0].name, std::string("open"));
    ASSERT_TRUE(componentType.signalsIo.outputs.empty());
}

TEST(ComponentsLibraryTest, LibraryCreation) {
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

    ASSERT_EQ(library.id, std::string("lib1"));
    ASSERT_EQ(library.name, std::string("Standard Library"));
    ASSERT_EQ(library.version, std::string("1.0.0"));
    ASSERT_EQ(library.author, std::string("Author"));
    ASSERT_EQ(library.description, std::string("A test library description"));
    ASSERT_EQ(library.portTypes.size(), 1);
    ASSERT_EQ(library.portTypes[0].name, std::string("LiquidPort"));
    ASSERT_EQ(library.components.size(), 1);
    ASSERT_EQ(library.components[0].name, std::string("Valve"));
}
