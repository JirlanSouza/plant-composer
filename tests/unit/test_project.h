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
#include <QDir>
#include <QFile>
#include <QDebug>

#include "domain/project/model/project.h"
#include "adapters/project/flat_buffer_project_loader.h"

namespace dp = domain::project;
namespace ap = adapters::project;

class TestProject : public QObject {
    Q_OBJECT

private slots:
    void testProjectCreation() {
        dp::Project project(
            "proj1",
            "My Project",
            "A test project",
            "Test Author",
            "1.0.0",
            "/tmp/my_project"
        );

        QCOMPARE(project.getId(), std::string("proj1"));
        QCOMPARE(project.getName(), std::string("My Project"));
        QCOMPARE(project.getDescription(), std::string("A test project"));
        QCOMPARE(project.getAuthor(), std::string("Test Author"));
        QCOMPARE(project.getVersion(), std::string("1.0.0"));
        QCOMPARE(project.getPath(), std::string("/tmp/my_project"));
        QVERIFY(project.getDiagramsMetadata().empty());
    }

    void testAddDiagramMetadata() {
        dp::Project project("proj2", "Another Project", "", "", "", "");
        dp::Project::DiagramMetadata metadata1 = {"diag1", "Diagram One", "diagrams/diag1.fbs"};
        dp::Project::DiagramMetadata metadata2 = {"diag2", "Diagram Two", "diagrams/diag2.fbs"};

        project.addDiagramMetadata(metadata1);
        project.addDiagramMetadata(metadata2);

        QCOMPARE(project.getDiagramsMetadata().size(), 2);
        QCOMPARE(project.getDiagramsMetadata()[0].id, std::string("diag1"));
        QCOMPARE(project.getDiagramsMetadata()[1].name, std::string("Diagram Two"));
    }

    void testSaveAndLoadProject() {
        QString tempDir = QDir::tempPath();
        QString filePath = tempDir + "/test_project.fbs";

        QFile::remove(filePath);

        dp::Project originalProject(
            "proj3",
            "Serializable Project",
            "This project should be saved and loaded.",
            "Serializer",
            "1.0.0",
            filePath.toStdString()
        );
        originalProject.addDiagramMetadata({"diagA", "Diagram A", "diagrams/diagA.fbs"});
        originalProject.addDiagramMetadata({"diagB", "Diagram B", "diagrams/diagB.fbs"});

        ap::FlatBufferProjectLoader loader;
        loader.saveProject(originalProject);

        std::unique_ptr<dp::Project> loadedProject = loader.loadProject(filePath.toStdString());

        QVERIFY(loadedProject != nullptr);
        QCOMPARE(loadedProject->getId(), originalProject.getId());
        QCOMPARE(loadedProject->getName(), originalProject.getName());
        QCOMPARE(loadedProject->getDescription(), originalProject.getDescription());
        QCOMPARE(loadedProject->getAuthor(), originalProject.getAuthor());
        QCOMPARE(loadedProject->getVersion(), originalProject.getVersion());
        QCOMPARE(loadedProject->getPath(), originalProject.getPath());

        QCOMPARE(loadedProject->getDiagramsMetadata().size(), originalProject.getDiagramsMetadata().size());
        if (loadedProject->getDiagramsMetadata().size() == originalProject.getDiagramsMetadata().size()) {
            for (size_t i = 0; i < loadedProject->getDiagramsMetadata().size(); ++i) {
                QCOMPARE(loadedProject->getDiagramsMetadata()[i].id, originalProject.getDiagramsMetadata()[i].id);
                QCOMPARE(loadedProject->getDiagramsMetadata()[i].name, originalProject.getDiagramsMetadata()[i].name);
                QCOMPARE(
                    loadedProject->getDiagramsMetadata()[i].filePath,
                    originalProject.getDiagramsMetadata()[i].filePath
                );
            }
        }

        QFile::remove(filePath);
    }
};
