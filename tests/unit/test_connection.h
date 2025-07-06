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
#include "domain/diagram/model/connection.h"

class TestConnection : public QObject {
    Q_OBJECT

private slots:
    void testConnectionCreation() {
        domain::diagram::Connection connection(
            "conn1",
            domain::diagram::Connection::Type::Port,
            "comp1",
            "portA",
            "comp2",
            "portB"
        );

        QCOMPARE(connection.getId(), std::string("conn1"));
        QCOMPARE(connection.getType(), domain::diagram::Connection::Type::Port);
        QCOMPARE(connection.getSourceComponentId(), std::string("comp1"));
        QCOMPARE(connection.getSourceIoId(), std::string("portA"));
        QCOMPARE(connection.getTargetComponentId(), std::string("comp2"));
        QCOMPARE(connection.getTargetIoId(), std::string("portB"));
    }

    void testSignalConnectionType() {
        domain::diagram::Connection connection(
            "conn2",
            domain::diagram::Connection::Type::Signal,
            "comp3",
            "signalX",
            "comp4",
            "signalY"
        );

        QCOMPARE(connection.getType(), domain::diagram::Connection::Type::Signal);
    }
};
