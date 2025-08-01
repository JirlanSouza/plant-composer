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

#include "domain/diagram/model/diagram.h"
#include "domain/diagram/model/component_instance.h"
#include "domain/diagram/model/connection.h"
#include "domain/diagram/model/node_transform.h"
#include "domain/components_library/model/component_type.h"

namespace diag = diagram;
namespace dcl = components_library;

TEST(DiagramTest, DiagramCreation) {
    diag::Diagram diagram("diag1", "My Diagram");

    ASSERT_EQ(diagram.getId(), std::string("diag1"));
    ASSERT_EQ(diagram.getName(), std::string("My Diagram"));
    ASSERT_EQ(diagram.componentsCount(), 0);
    ASSERT_EQ(diagram.connectionsCount(), 0);
}

TEST(DiagramTest, AddComponent) {
    diag::Diagram diagram("diag2", "Another Diagram");

    dcl::ComponentType componentType = {
        "type1",
        "Valve",
        "",
        "",
        {},
        {{}, {}},
        {{}, {}}
    };
    diag::NodeTransform transform = {0, 0, 0};
    diag::ComponentInstance componentInstance("comp1", "Valve 1", "type1", transform, &componentType, "");

    diagram.addComponent(componentInstance);

    ASSERT_EQ(diagram.componentsCount(), 1);
    ASSERT_EQ(diagram.getComponents()[0].getId(), std::string("comp1"));
}

TEST(DiagramTest, AddConnection) {
    diag::Diagram diagram("diag3", "Diagram with Connections");

    diag::Connection connection(
        "conn1",
        diag::Connection::Type::Port,
        "compA",
        "portX",
        "compB",
        "portY"
    );

    diagram.addConnection(connection);

    ASSERT_EQ(diagram.connectionsCount(), 1);
    ASSERT_EQ(diagram.getConnections()[0].getId(), std::string("conn1"));
}

TEST(DiagramTest, AddMultipleComponentsAndConnections) {
    diag::Diagram diagram("diag4", "Complex Diagram");

    dcl::ComponentType componentType1 = {
        "typeA",
        "Pump",
        "",
        "",
        {},
        {{}, {}},
        {{}, {}}
    };
    dcl::ComponentType componentType2 = {
        "typeB",
        "Tank",
        "",
        "",
        {},
        {{}, {}},
        {{}, {}}
    };

    diag::NodeTransform transform1 = {0, 0, 0};
    diag::NodeTransform transform2 = {10, 20, 0};

    diag::ComponentInstance comp1("compA", "Pump 1", "typeA", transform1, &componentType1, "");
    diag::ComponentInstance comp2("compB", "Tank 1", "typeB", transform2, &componentType2, "");

    diagram.addComponent(comp1);
    diagram.addComponent(comp2);

    diag::Connection conn1("connA", diag::Connection::Type::Port, "compA", "out", "compB", "in");
    diag::Connection conn2("connB", diag::Connection::Type::Signal, "compA", "status", "compB", "control");

    diagram.addConnection(conn1);
    diagram.addConnection(conn2);

    ASSERT_EQ(diagram.componentsCount(), 2);
    ASSERT_EQ(diagram.connectionsCount(), 2);

    ASSERT_EQ(diagram.getComponents()[0].getName(), std::string("Pump 1"));
    ASSERT_EQ(diagram.getComponents()[1].getName(), std::string("Tank 1"));

    ASSERT_EQ(diagram.getConnections()[0].getSourceComponentId(), std::string("compA"));
    ASSERT_EQ(diagram.getConnections()[1].getType(), diag::Connection::Type::Signal);
}
