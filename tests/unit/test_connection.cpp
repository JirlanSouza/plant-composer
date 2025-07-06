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

#include "domain/diagram/model/connection.h"

namespace dd = domain::diagram;

TEST(ConnectionTest, ConnectionCreation) {
    dd::Connection connection(
        "conn1",
        dd::Connection::Type::Port,
        "comp1",
        "portA",
        "comp2",
        "portB"
    );

    ASSERT_EQ(connection.getId(), std::string("conn1"));
    ASSERT_EQ(connection.getType(), dd::Connection::Type::Port);
    ASSERT_EQ(connection.getSourceComponentId(), std::string("comp1"));
    ASSERT_EQ(connection.getSourceIoId(), std::string("portA"));
    ASSERT_EQ(connection.getTargetComponentId(), std::string("comp2"));
    ASSERT_EQ(connection.getTargetIoId(), std::string("portB"));
}

TEST(ConnectionTest, SignalConnectionType) {
    dd::Connection connection(
        "conn2",
        dd::Connection::Type::Signal,
        "comp3",
        "signalX",
        "comp4",
        "signalY"
    );

    ASSERT_EQ(connection.getType(), dd::Connection::Type::Signal);
}
