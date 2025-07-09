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

#include "new_diagram_dialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include "components_library/properties_dialog/component_properties_view.h"

namespace ui::diagram_editor {
    NewDiagramDialog::NewDiagramDialog(QWidget *parent)
        : QDialog(parent) {
        setWindowTitle(tr("New Diagram"));
        setModal(true);
        setMinimumSize(400, 200);

        auto *mainLayout = new QVBoxLayout(this);

        auto *formLayout = new QFormLayout();
        auto *label = new QLabel(tr("Enter the name of the new diagram:"), this);
        QLineEdit *lineEdit = new QLineEdit(this);

        formLayout->addRow(label);
        formLayout->addRow(lineEdit);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        auto *okButton = buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(false);

        mainLayout->addLayout(formLayout);
        mainLayout->addStretch(); // empurra o buttonBox para baixo
        mainLayout->addWidget(buttonBox);

        connect(
            lineEdit,
            &QLineEdit::textChanged,
            this,
            [okButton](const QString &text) {
                okButton->setEnabled(!text.isEmpty());
            }
        );
        connect(
            buttonBox,
            &QDialogButtonBox::accepted,
            this,
            [this, lineEdit]() {
                emit diagramNameEntered(lineEdit->text().toStdString());
            }
        );
        connect(buttonBox, &QDialogButtonBox::rejected, this, &NewDiagramDialog::reject);
    }

    NewDiagramDialog::~NewDiagramDialog() = default;

    int NewDiagramDialog::execForName(std::string diagramName) {
        return QDialog::exec();
    }
}
