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

#include <QtTest/QTest>

#include "test_connection.h"
#include "test_project.h"
#include "test_diagram.h"
#include "test_components_library.h"

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    int status = 0; {
        TestConnection tc;
        status |= QTest::qExec(&tc, argc, argv);
    } {
        TestProject tp;
        status |= QTest::qExec(&tp, argc, argv);
    }
    {
        TestDiagram td;
        status |= QTest::qExec(&td, argc, argv);
    }
    {
        TestComponentsLibrary tcl;
        status |= QTest::qExec(&tcl, argc, argv);
    }

    return status;
}
