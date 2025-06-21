#include "library_component_properties_view.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QLabel>


LibraryComponentPropertiesView::LibraryComponentPropertiesView(QWidget *parent,
                                                               const Component &component) : QDialog(parent),
    component_(component) {
    setWindowTitle(QString::fromStdString(component_.name));
    resize(600, 600);
    buildUI();
}

void LibraryComponentPropertiesView::buildUI() {
    auto *mainLayout = new QVBoxLayout(this);
    buildConstantsView(mainLayout);
    buildInputsView(mainLayout);
    buildOutputsView(mainLayout);
}

QTableWidget *LibraryComponentPropertiesView::buildPropertyView(QLayout *mainLayout, const std::string &groupName,
                                                                const int tableRows, const int tableColumns) {
    auto *group = new QGroupBox(QString::fromStdString(groupName));
    auto *layout = new QVBoxLayout(group);
    auto *table = new QTableWidget(tableRows, tableColumns);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(group);
    layout->addWidget(table);
    return table;
}

void LibraryComponentPropertiesView::buildConstantsView(QLayout *layout) const {
    QTableWidget *constantsTable = buildPropertyView(layout, "Constants", component_.constants.size(), 3);
    constantsTable->setHorizontalHeaderLabels({"Name", "Data type", "Default value"});

    for (int i = 0; i < component_.constants.size(); ++i) {
        constantsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(component_.constants[i].name)));
        constantsTable->setItem(
            i, 1, new QTableWidgetItem(QString::fromStdString(to_string(component_.constants[i].dataType))));
        constantsTable->setItem(
            i, 2, new QTableWidgetItem(QString::fromStdString(component_.constants[i].defaultValue)));
    }
}

void LibraryComponentPropertiesView::buildInputsView(QLayout *layout) const {
    auto *inputsTable = buildPropertyView(layout, "Inputs", component_.inputs.size(), 2);
    inputsTable->setHorizontalHeaderLabels({"Name", "Data type"});

    for (int i = 0; i < component_.inputs.size(); ++i) {
        inputsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(component_.inputs[i].name)));
        inputsTable->setItem(
            i, 1, new QTableWidgetItem(QString::fromStdString(to_string(component_.inputs[i].dataType))));
    }
}

void LibraryComponentPropertiesView::buildOutputsView(QLayout *layout) const {
    auto *outputsTable = buildPropertyView(layout, "Outputs", component_.outputs.size(), 2);
    outputsTable->setHorizontalHeaderLabels({"Name", "Data type"});

    for (int i = 0; i < component_.outputs.size(); ++i) {
        outputsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(component_.outputs[i].name)));
        outputsTable->setItem(
            i, 1, new QTableWidgetItem(QString::fromStdString(to_string(component_.outputs[i].dataType))));
    }
}
