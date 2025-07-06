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

#include "domain/diagram/model/diagram.h"
#include "domain/diagram/model/component_instance.h"
#include "domain/diagram/model/connection.h"
#include "domain/diagram/model/node_transform.h"
#include "domain/components_library/model/component_type.h"

namespace dd = domain::diagram;
namespace dcl = domain::components_library;

class TestDiagram : public QObject {
    Q_OBJECT

private slots:
    void testDiagramCreation() {
        dd::Diagram diagram("diag1", "My Diagram");

        QCOMPARE(diagram.getId(), std::string("diag1"));
        QCOMPARE(diagram.getName(), std::string("My Diagram"));
        QCOMPARE(diagram.componentsCount(), 0);
        QCOMPARE(diagram.connectionsCount(), 0);
    }

    void testAddComponent() {
        dd::Diagram diagram("diag2", "Another Diagram");

        dcl::ComponentType componentType = {
            "type1",
            "Valve",
            "",
            "",
            {},
            {{}, {}},
            {{}, {}}
        };
        dd::NodeTransform transform = {0, 0, 0};
        dd::ComponentInstance componentInstance("comp1", "Valve 1", "type1", transform, &componentType, "");

        diagram.addComponent(componentInstance);

        QCOMPARE(diagram.componentsCount(), 1);
        QCOMPARE(diagram.getComponents()[0].getId(), std::string("comp1"));
    }

    void testAddConnection() {
        dd::Diagram diagram("diag3", "Diagram with Connections");

        dd::Connection connection(
            "conn1",
            dd::Connection::Type::Port,
            "compA",
            "portX",
            "compB",
            "portY"
        );

        diagram.addConnection(connection);

        QCOMPARE(diagram.connectionsCount(), 1);
        QCOMPARE(diagram.getConnections()[0].getId(), std::string("conn1"));
    }

    void testAddMultipleComponentsAndConnections() {
        dd::Diagram diagram("diag4", "Complex Diagram");

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

        dd::NodeTransform transform1 = {0, 0, 0};
        dd::NodeTransform transform2 = {10, 20, 0};

        dd::ComponentInstance comp1("compA", "Pump 1", "typeA", transform1, &componentType1, "");
        dd::ComponentInstance comp2("compB", "Tank 1", "typeB", transform2, &componentType2, "");

        diagram.addComponent(comp1);
        diagram.addComponent(comp2);

        dd::Connection conn1("connA", dd::Connection::Type::Port, "compA", "out", "compB", "in");
        dd::Connection conn2("connB", dd::Connection::Type::Signal, "compA", "status", "compB", "control");

        diagram.addConnection(conn1);
        diagram.addConnection(conn2);

        QCOMPARE(diagram.componentsCount(), 2);
        QCOMPARE(diagram.connectionsCount(), 2);

        QCOMPARE(diagram.getComponents()[0].getName(), std::string("Pump 1"));
        QCOMPARE(diagram.getComponents()[1].getName(), std::string("Tank 1"));

        QCOMPARE(diagram.getConnections()[0].getSourceComponentId(), std::string("compA"));
        QCOMPARE(diagram.getConnections()[1].getType(), dd::Connection::Type::Signal);
    }
};
