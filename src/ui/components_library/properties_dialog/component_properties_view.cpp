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

#include "component_properties_view.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QLabel>

namespace ui::components_library {
    LibraryComponentPropertiesView::LibraryComponentPropertiesView(
        QWidget *parent,
        const dcl::Component &component,
        std::unordered_map<int, dcl::PortType>
        &portTypes
    ) : QDialog(parent),
        component_(component),
        portTypes_(portTypes) {
        setWindowTitle(QString::fromStdString(component_.name));
        resize(600, 600);
        buildUI();
    }

    void LibraryComponentPropertiesView::buildUI() {
        auto *mainLayout = new QVBoxLayout(this);
        buildConstantsView(mainLayout);
        buildPortsView(mainLayout);
        buildSignalsView(mainLayout);
    }

    QLayout *LibraryComponentPropertiesView::buildGroupLayout(QLayout *parentLayout, const std::string &groupName) {
        auto *group = new QGroupBox(QString::fromStdString(groupName));
        auto *layout = new QVBoxLayout(group);
        parentLayout->addWidget(group);
        return layout;
    }

    QTableWidget *LibraryComponentPropertiesView::buildTable(
        QLayout *parentLayout,
        int tableRows,
        const QStringList &headers
    ) {
        auto *table = new QTableWidget(tableRows, headers.size());
        table->setHorizontalHeaderLabels(headers);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        parentLayout->addWidget(table);
        return table;
    }


    void LibraryComponentPropertiesView::buildConstantsView(QLayout *layout) const {
        const auto group = buildGroupLayout(layout, "Constants");
        const auto table = buildTable(
            group,
            component_.constants.size(),
            {"Name", "Data type", "Default value"}
        );

        for (int i = 0; i < component_.constants.size(); ++i) {
            table->setItem(i, 0, newTableItem(component_.constants[i].name));
            table->setItem(
                i,
                1,
                newTableItem(dataTypeToString(component_.constants[i].dataType))
            );
            table->setItem(
                i,
                2,
                newTableItem(component_.constants[i].defaultValue)
            );
        }
    }

    void LibraryComponentPropertiesView::buildPortsView(QLayout *mainLayout) const {
        const auto group = buildGroupLayout(mainLayout, "Ports");
        const auto inputsGroup = buildGroupLayout(group, "Inputs");
        const auto outputsGroup = buildGroupLayout(group, "Outputs");

        const auto headers = QStringList({"Name", "Type"});
        const auto inputsTable = buildTable(inputsGroup, component_.ports.inputs.size(), headers);
        const auto outputsTable = buildTable(outputsGroup, component_.ports.outputs.size(), headers);


        for (int i = 0; i < component_.ports.inputs.size(); ++i) {
            inputsTable->setItem(i, 0, newTableItem(component_.ports.inputs[i].name));
            inputsTable->setItem(
                i,
                1,
                newTableItem(
                    portTypes_.at(component_.ports.inputs[i].typeId).name
                )
            );
        }

        for (int i = 0; i < component_.ports.outputs.size(); ++i) {
            outputsTable->setItem(i, 0, newTableItem(component_.ports.outputs[i].name));
            outputsTable->setItem(
                i,
                1,
                newTableItem(
                    portTypes_.at(component_.ports.outputs[i].typeId).name
                )
            );
        }
    }

    void LibraryComponentPropertiesView::buildSignalsView(QLayout *mainLayout) const {
        const auto group = buildGroupLayout(mainLayout, "Signals");
        const auto inputsGroup = buildGroupLayout(group, "Inputs");
        const auto outputsGroup = buildGroupLayout(group, "Outputs");

        const auto headers = QStringList({"Name", "Type"});
        const auto inputsTable = buildTable(inputsGroup, component_.signalsIo.inputs.size(), headers);
        const auto outputsTable = buildTable(outputsGroup, component_.signalsIo.outputs.size(), headers);


        for (int i = 0; i < component_.signalsIo.inputs.size(); ++i) {
            inputsTable->setItem(i, 0, newTableItem(component_.signalsIo.inputs[i].name));
            inputsTable->setItem(
                i,
                1,
                newTableItem(
                    dcl::dataTypeToString(component_.signalsIo.inputs[i].dataType)
                )
            );
        }

        for (int i = 0; i < component_.signalsIo.outputs.size(); ++i) {
            outputsTable->setItem(i, 0, newTableItem(component_.signalsIo.outputs[i].name));
            outputsTable->setItem(
                i,
                1,
                newTableItem(
                    dcl::dataTypeToString(component_.signalsIo.inputs[i].dataType)
                )
            );
        }
    }

    QTableWidgetItem *LibraryComponentPropertiesView::newTableItem(const std::string &strValue) {
        return new QTableWidgetItem(QString::fromStdString(strValue));
    }
}
