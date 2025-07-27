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

#include "domain/project/model/project.h"

namespace dp = project;

TEST(ProjectTest, ProjectCreation) {
    dp::Project project(
        "proj1",
        "My Project",
        "A test project",
        "Test Author",
        "1.0.0",
        "/tmp/my_project"
    );

    ASSERT_EQ(project.getId(), std::string("proj1"));
    ASSERT_EQ(project.getName(), std::string("My Project"));
    ASSERT_EQ(project.getDescription(), std::string("A test project"));
    ASSERT_EQ(project.getAuthor(), std::string("Test Author"));
    ASSERT_EQ(project.getVersion(), std::string("1.0.0"));
    ASSERT_EQ(project.getPath(), std::string("/tmp/my_project"));
    ASSERT_TRUE(project.getDiagramsMetadata().empty());
}

TEST(ProjectTest, AddDiagramMetadata) {
    dp::Project project("proj2", "Another Project", "", "", "", "");
    dp::Project::DiagramMetadata metadata1 = {"diag1", "Diagram One", "diagrams/diag1.fbs"};
    dp::Project::DiagramMetadata metadata2 = {"diag2", "Diagram Two", "diagrams/diag2.fbs"};

    project.addDiagramMetadata(metadata1);
    project.addDiagramMetadata(metadata2);

    ASSERT_EQ(project.getDiagramsMetadata().size(), 2);
    ASSERT_EQ(project.getDiagramsMetadata()[0].id, std::string("diag1"));
    ASSERT_EQ(project.getDiagramsMetadata()[1].name, std::string("Diagram Two"));
}
