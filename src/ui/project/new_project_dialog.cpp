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

#include "new_project_dialog.h"

namespace ui::project {
    NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent) {
        setWindowTitle(tr("Create New Project"));
        setMinimumSize(400, 300);
        setupUi();
        setupConnections();
    }

    void NewProjectDialog::setupUi() {
        auto *mainLayout = new QVBoxLayout(this);

        auto *projectNameLayout = new QHBoxLayout();
        projectNameLayout->addWidget(new QLabel(tr("Project Name:")));
        projectNameLineEdit_ = new QLineEdit();
        projectNameLayout->addWidget(projectNameLineEdit_);
        mainLayout->addLayout(projectNameLayout);

        auto *projectDescriptionLayout = new QVBoxLayout();
        projectDescriptionLayout->addWidget(new QLabel(tr("Description:")));
        projectDescriptionTextEdit_ = new QTextEdit();
        projectDescriptionLayout->addWidget(projectDescriptionTextEdit_);
        mainLayout->addLayout(projectDescriptionLayout);

        auto *projectAuthorLayout = new QHBoxLayout();
        projectAuthorLayout->addWidget(new QLabel(tr("Author:")));
        projectAuthorLineEdit_ = new QLineEdit();
        projectAuthorLayout->addWidget(projectAuthorLineEdit_);
        mainLayout->addLayout(projectAuthorLayout);

        auto *projectPathLayout = new QHBoxLayout();
        projectPathLayout->addWidget(new QLabel(tr("Path:")));
        projectPathLineEdit_ = new QLineEdit();
        projectPathLayout->addWidget(projectPathLineEdit_);
        mainLayout->addLayout(projectPathLayout);

        auto *buttonLayout = new QHBoxLayout();
        createButton_ = new QPushButton(tr("Create"));
        cancelButton_ = new QPushButton(tr("Cancel"));
        buttonLayout->addStretch();
        buttonLayout->addWidget(createButton_);
        buttonLayout->addWidget(cancelButton_);
        mainLayout->addLayout(buttonLayout);
    }

    void NewProjectDialog::setupConnections() {
        connect(createButton_, &QPushButton::clicked, this, &NewProjectDialog::accept);
        connect(cancelButton_, &QPushButton::clicked, this, &NewProjectDialog::reject);
    }

    QString NewProjectDialog::getProjectName() const {
        return projectNameLineEdit_->text();
    }

    QString NewProjectDialog::getProjectDescription() const {
        return projectDescriptionTextEdit_->toPlainText();
    }

    QString NewProjectDialog::getProjectAuthor() const {
        return projectAuthorLineEdit_->text();
    }

    QString NewProjectDialog::getProjectPath() const {
        return projectPathLineEdit_->text();
    }
}
