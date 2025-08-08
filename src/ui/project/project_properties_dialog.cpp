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

#include "project_properties_dialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>

namespace project {
    ProjectPropertiesDialog::ProjectPropertiesDialog(::project::Project *project, QWidget *parent)
        : QDialog(parent) {
        setWindowTitle(tr("Project Properties"));

        auto *mainLayout = new QVBoxLayout(this);
        auto *formLayout = new QFormLayout();

        nameLineEdit_ = new QLineEdit(QString::fromStdString(project->getName()), this);
        descriptionTextEdit_ = new QTextEdit(QString::fromStdString(project->getDescription()), this);

        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout->addRow(tr("Name:"), nameLineEdit_);
        formLayout->addRow(tr("Description:"), descriptionTextEdit_);
        formLayout->addRow(tr("Author:"), new QLabel(QString::fromStdString(project->getAuthor()), this));
        formLayout->addRow(tr("Version:"), new QLabel(QString::fromStdString(project->getVersion()), this));
        formLayout->addRow(tr("Path:"), new QLabel(QString::fromStdString(project->getPath()), this));
        mainLayout->addLayout(formLayout);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        mainLayout->addWidget(buttonBox);
    }

    QString ProjectPropertiesDialog::getName() const {
        return nameLineEdit_->text();
    }

    QString ProjectPropertiesDialog::getDescription() const {
        return descriptionTextEdit_->toPlainText();
    }
}
