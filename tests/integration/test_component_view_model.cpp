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
#include <QObject>
#include <QPointF>

#include "ui/diagram_editor/editor_widget/component_view_model.h"
#include "domain/diagram/model/component_instance.h"
#include "domain/diagram/model/node_transform.h"
#include "domain/components_library/model/component_type.h"

namespace diag = diagram;
namespace diag = diagram;
namespace dcl = components_library;

class ComponentViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        componentType = {
            "type1",
            "TestComponent",
            "",
            "",
            {},
            {{}, {}},
            {{}, {}}
        };

        nodeTransform = {10, 20, 0};
        componentInstance = std::make_unique<diag::ComponentInstance>(
            "comp1",
            "Instance1",
            "type1",
            nodeTransform,
            &componentType,
            "/assets"
        );

        viewModel = std::make_unique<diag::ComponentViewModel>(componentInstance.get(), nullptr);
    }

    dcl::ComponentType componentType;
    diag::NodeTransform nodeTransform;
    std::unique_ptr<diag::ComponentInstance> componentInstance;
    std::unique_ptr<diag::ComponentViewModel> viewModel;
};

TEST_F(ComponentViewModelTest, Initialization) {
    ASSERT_EQ(viewModel->getId().toStdString(), componentInstance->getId());
    ASSERT_EQ(viewModel->getName().toStdString(), componentInstance->getName());
    ASSERT_EQ(viewModel->getSymbolPath().toStdString(), componentInstance->getSymbolPath());
    ASSERT_EQ(viewModel->getPosition().x(), componentInstance->getPosition().x);
    ASSERT_EQ(viewModel->getPosition().y(), componentInstance->getPosition().y);
}

TEST_F(ComponentViewModelTest, SetNameUpdatesDomainModel) {
    viewModel->setName("new name");

    ASSERT_EQ(componentInstance->getName(), "new name");
}

TEST_F(ComponentViewModelTest, NameChangedSignal) {
    bool signalEmitted = false;
    QObject::connect(
        viewModel.get(),
        &diag::ComponentViewModel::nameChanged,
        [&]() {
            signalEmitted = true;
        }
    );

    viewModel->setName("name");
    ASSERT_TRUE(signalEmitted);
}

TEST_F(ComponentViewModelTest, SetPositionUpdatesDomainModel) {
    QPointF newPos(50.0, 60.0);
    viewModel->setPosition(newPos);

    ASSERT_EQ(componentInstance->getPosition().x, newPos.x());
    ASSERT_EQ(componentInstance->getPosition().y, newPos.y());
}

TEST_F(ComponentViewModelTest, PositionChangedSignal) {
    bool signalEmitted = false;
    QObject::connect(
        viewModel.get(),
        &diag::ComponentViewModel::positionChanged,
        [&]() {
            signalEmitted = true;
        }
    );

    viewModel->setPosition(QPointF(100.0, 110.0));
    ASSERT_TRUE(signalEmitted);
}
