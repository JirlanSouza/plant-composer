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

#include "domain/project/model/project.h"
#include "adapters/project/flat_buffer_project_loader.h"

namespace dp = domain::project;
namespace ap = adapters::project;

class ProjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        int argc = 0;
        static QCoreApplication app(argc, nullptr);

        filePath = QDir::tempPath().toStdString() + "/test_project.fbs";
        QFile::remove(QString::fromStdString(filePath));
    }

    void TearDown() override {
        QFile::remove(QString::fromStdString(filePath));
    }

    std::string filePath;
};

TEST_F(ProjectTest, ProjectCreation) {
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

TEST_F(ProjectTest, AddDiagramMetadata) {
    dp::Project project("proj2", "Another Project", "", "", "", "");
    dp::Project::DiagramMetadata metadata1 = {"diag1", "Diagram One", "diagrams/diag1.fbs"};
    dp::Project::DiagramMetadata metadata2 = {"diag2", "Diagram Two", "diagrams/diag2.fbs"};

    project.addDiagramMetadata(metadata1);
    project.addDiagramMetadata(metadata2);

    ASSERT_EQ(project.getDiagramsMetadata().size(), 2);
    ASSERT_EQ(project.getDiagramsMetadata()[0].id, std::string("diag1"));
    ASSERT_EQ(project.getDiagramsMetadata()[1].name, std::string("Diagram Two"));
}

TEST_F(ProjectTest, SaveAndLoadProject) {
    dp::Project originalProject(
        "proj3",
        "Serializable Project",
        "This project should be saved and loaded.",
        "Serializer",
        "1.0.0",
        filePath
    );
    originalProject.addDiagramMetadata({"diagA", "Diagram A", "diagrams/diagA.fbs"});
    originalProject.addDiagramMetadata({"diagB", "Diagram B", "diagrams/diagB.fbs"});


    ap::FlatBufferProjectLoader loader;
    loader.saveProject(originalProject);


    std::unique_ptr<dp::Project> loadedProject = loader.loadProject(filePath);


    ASSERT_NE(loadedProject, nullptr);
    ASSERT_EQ(loadedProject->getId(), originalProject.getId());
    ASSERT_EQ(loadedProject->getName(), originalProject.getName());
    ASSERT_EQ(loadedProject->getDescription(), originalProject.getDescription());
    ASSERT_EQ(loadedProject->getAuthor(), originalProject.getAuthor());
    ASSERT_EQ(loadedProject->getVersion(), originalProject.getVersion());
    ASSERT_EQ(loadedProject->getPath(), originalProject.getPath());

    ASSERT_EQ(loadedProject->getDiagramsMetadata().size(), originalProject.getDiagramsMetadata().size());
    if (loadedProject->getDiagramsMetadata().size() == originalProject.getDiagramsMetadata().size()) {
        for (size_t i = 0; i < loadedProject->getDiagramsMetadata().size(); ++i) {
            ASSERT_EQ(loadedProject->getDiagramsMetadata()[i].id, originalProject.getDiagramsMetadata()[i].id);
            ASSERT_EQ(loadedProject->getDiagramsMetadata()[i].name, originalProject.getDiagramsMetadata()[i].name);
            ASSERT_EQ(
                loadedProject->getDiagramsMetadata()[i].filePath,
                originalProject.getDiagramsMetadata()[i].filePath
            );
        }
    }
}
