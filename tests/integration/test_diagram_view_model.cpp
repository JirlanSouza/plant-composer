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
#include "gmock/gmock.h"
#include <QObject>
#include <QPointF>

#include "ui/diagram_editor/editor_widget/diagram_view_model.h"
#include "domain/diagram/model/diagram.h"
#include "domain/diagram/component_instance_factory.h"
#include "domain/diagram/model/component_instance.h"
#include "domain/components_library/model/component_type.h"
#include "domain/shared/id_factory.h"
#include "domain/settings/app_settings.h"

using ::testing::Return;
using ::testing::An;

class MockIDFactory : public domain::IDFactory {
public:
    MOCK_METHOD(std::string, newId, (), (override));
};


class MockAppSettings : public domain::settings::AppSettings {
public:
    MOCK_METHOD(std::string, getAssetsDir, (), (override));
    MOCK_METHOD(void, setAssetsDir, (const std::string &assetsDir), (override));
    MOCK_METHOD(std::string, getComponentsLibraryDir, (), (override));
    MOCK_METHOD(std::string, getComponentIconPath, (), (override));
};

namespace ui_de = ui::diagram_editor;
namespace dd = domain::diagram;
namespace dcl = domain::components_library;

class DiagramViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockIdFactory = std::make_unique<MockIDFactory>();
        mockAppSettings = std::make_unique<MockAppSettings>();
        dummyLibraries = std::make_unique<std::vector<dcl::Library> >();


        dcl::ComponentType dummyComponentTypeInLibrary = {
            "dummy_type",
            "Dummy Component In Library",
            "",
            "",
            {},
            {{}, {}},
            {{}, {}}
        };
        dcl::Library dummyLibrary = {
            "dummy_lib_id",
            "Dummy Library",
            "1.0.0",
            "",
            "",
            {},
            {dummyComponentTypeInLibrary}
        };

        dummyLibraries->push_back(dummyLibrary);
        diagram = std::make_unique<dd::Diagram>("test_id", "Test Diagram");
        componentInstanceFactory = std::make_unique<dd::ComponentInstanceFactory>(
            mockIdFactory.get(),
            dummyLibraries.get(),
            mockAppSettings.get()
        );

        dummyComponentInstance = std::make_unique<dd::ComponentInstance>(
            "dummy_instance_id",
            "Dummy Instance Name",
            "dummy_type",
            dd::NodeTransform{0, 0, 0},
            &dummyComponentTypeInLibrary,
            ""
        );

        viewModel = std::make_unique<ui_de::DiagramViewModel>(diagram.get(), componentInstanceFactory.get(), nullptr);
    }

    std::unique_ptr<MockIDFactory> mockIdFactory;
    std::unique_ptr<MockAppSettings> mockAppSettings;
    std::unique_ptr<std::vector<dcl::Library> > dummyLibraries;

    std::unique_ptr<dd::Diagram> diagram;
    std::unique_ptr<dd::ComponentInstanceFactory> componentInstanceFactory;
    std::unique_ptr<ui_de::DiagramViewModel> viewModel;

    std::unique_ptr<dd::ComponentInstance> dummyComponentInstance;
};

TEST_F(DiagramViewModelTest, Initialization) {
    ASSERT_NE(viewModel, nullptr);
    ASSERT_EQ(viewModel->componentsCount(), 0);
}

TEST_F(DiagramViewModelTest, ValidateDiagramInfo) {
    ASSERT_EQ(viewModel->getId(), diagram->getId());
    ASSERT_EQ(viewModel->getName(), diagram->getName());
}

TEST_F(DiagramViewModelTest, AddComponent) {
    EXPECT_CALL(*mockIdFactory, newId()).WillOnce(Return("new_component_id"));

    bool componentAddedSignalEmitted = false;
    QObject::connect(
        viewModel.get(),
        &ui_de::DiagramViewModel::componentAdded,
        [&](ui_de::ComponentViewModel *vm) {
            componentAddedSignalEmitted = true;
        }
    );


    viewModel->addComponent(0, 0, QPointF(10, 20));


    ASSERT_EQ(diagram->componentsCount(), 1);
    ASSERT_EQ(viewModel->componentsCount(), 1);
    ASSERT_NE(viewModel->getLastAddedComponentViewModel(), nullptr);
    ASSERT_EQ(viewModel->getLastAddedComponentViewModel()->getId().toStdString(), std::string("new_component_id"));
    ASSERT_TRUE(componentAddedSignalEmitted);
}

TEST_F(DiagramViewModelTest, GetLastAddedComponentViewModel) {
    EXPECT_CALL(*mockIdFactory, newId()).WillOnce(Return("new_component_id_2"));

    viewModel->addComponent(0, 0, QPointF(10, 20));

    ASSERT_NE(viewModel->getLastAddedComponentViewModel(), nullptr);
    ASSERT_EQ(viewModel->getLastAddedComponentViewModel()->getId().toStdString(), std::string("new_component_id_2"));
}

TEST_F(DiagramViewModelTest, AddMultipleComponents) {
    EXPECT_CALL(*mockIdFactory, newId())
        .WillOnce(Return("new_component_id_3"))
        .WillOnce(Return("new_component_id_4"));

    viewModel->addComponent(0, 0, QPointF(10, 20));
    viewModel->addComponent(0, 0, QPointF(30, 40));

    ASSERT_EQ(diagram->componentsCount(), 2);
    ASSERT_EQ(viewModel->componentsCount(), 2);
}
