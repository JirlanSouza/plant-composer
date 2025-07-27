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

/*
 * plant-composer
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

#include <QDialog>
#include <QGroupBox>
#include <QTableWidget>

#include "domain/components_library/model/component_type.h"

namespace components_library {
    class LibraryComponentPropertiesView final : public QDialog {
        Q_OBJECT

    public:
        LibraryComponentPropertiesView(
            QWidget *parent,
            const ComponentType &component,
            std::unordered_map<int, PortType> &portTypes
        );

        ~LibraryComponentPropertiesView() override = default;

    private:
        const ComponentType &component_;
        std::unordered_map<int, PortType> &portTypes_;

        void buildUI();

        static QLayout *buildGroupLayout(QLayout *parentLayout, const std::string &groupName);

        static QTableWidget *buildTable(QLayout *parentLayout, int tableRows, const QStringList &headers);

        void buildConstantsView(QLayout *layout) const;

        void buildPortsView(QLayout *mainLayout) const;

        void buildSignalsView(QLayout *layout) const;

        static QTableWidgetItem *newTableItem(const std::string &strValue);
    };
}
