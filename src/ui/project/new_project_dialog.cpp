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
 * along with this program.  If not, see <https:
 */

#include "new_project_dialog.h"

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QPalette>

namespace project {
    NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent) {
        setWindowTitle(tr("Create New Project"));
        setMinimumSize(400, 300);

        projectNameLineEdit_ = new QLineEdit();
        projectDescriptionTextEdit_ = new QTextEdit();
        projectAuthorLineEdit_ = new QLineEdit();
        projectParentDirectoryLineEdit_ = new QLineEdit();

        invalidPalette_ = projectNameLineEdit_->palette();
        invalidPalette_.setColor(QPalette::Text, Qt::red);
        invalidPalette_.setColor(QPalette::Base, QColor(255, 220, 220));
        projectNameErrorMessage_ = "";
        projectParentDirectoryErrorMessage_ = "";

        setupUi();
        setupConnections();
    }

    void NewProjectDialog::setupUi() {
        auto *mainLayout = new QVBoxLayout(this);
        auto *formLayout = new QFormLayout();

        formLayout->setVerticalSpacing(2);
        formLayout->setLabelAlignment(Qt::AlignJustify);
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        projectNameLineEdit_ = new QLineEdit();
        formLayout->addRow(tr("Project Name:"), projectNameLineEdit_);

        projectDescriptionTextEdit_ = new QTextEdit();
        formLayout->addRow(tr("Description:"), projectDescriptionTextEdit_);
        projectDescriptionTextEdit_->setMaximumHeight(50);

        projectAuthorLineEdit_ = new QLineEdit();
        projectAuthorLineEdit_->setText(QDir::home().dirName());
        formLayout->addRow(tr("Author:"), projectAuthorLineEdit_);

        projectParentDirectoryLineEdit_ = new QLineEdit();
        browsePathButton_ = new QPushButton(tr("Browse..."));
        auto *pathLayout = new QHBoxLayout();
        pathLayout->addWidget(projectParentDirectoryLineEdit_);
        pathLayout->addWidget(browsePathButton_);
        formLayout->addRow(tr("Path:"), pathLayout);

        mainLayout->addLayout(formLayout);

        errorMessageLabel_ = new QLabel(this);
        errorMessageLabel_->setStyleSheet("color: red;");
        errorMessageLabel_->setWordWrap(true);
        mainLayout->addWidget(errorMessageLabel_);

        auto *buttonLayout = new QHBoxLayout();
        cancelButton_ = new QPushButton(tr("Cancel"));
        createButton_ = new QPushButton(tr("Create"));
        createButton_->setDefault(true);
        createButton_->setEnabled(false);
        buttonLayout->addStretch();
        buttonLayout->addWidget(cancelButton_);
        buttonLayout->addWidget(createButton_);
        mainLayout->addLayout(buttonLayout);
    }

    void NewProjectDialog::setupConnections() {
        connect(createButton_, &QPushButton::clicked, this, &NewProjectDialog::accept);
        connect(cancelButton_, &QPushButton::clicked, this, &NewProjectDialog::reject);
        connect(browsePathButton_, &QPushButton::clicked, this, &NewProjectDialog::onBrowsePathClicked);

        connect(projectNameLineEdit_, &QLineEdit::textChanged, this, &NewProjectDialog::validateProjectNameInput);
        connect(
            projectParentDirectoryLineEdit_,
            &QLineEdit::textChanged,
            this,
            &NewProjectDialog::validateProjectParentDirectoryInput
        );
    }

    void NewProjectDialog::validateProjectNameInput() {
        isProjectNameValid_ = projectNameLineEdit_->text().length() >= 3;
        projectNameErrorMessage_.clear();

        if (!isProjectNameValid_ && projectNameIsDirty_) {
            projectNameErrorMessage_ = tr("Project Name must be at least 3 characters long.\n");
            projectNameLineEdit_->setPalette(invalidPalette_);
        } else {
            projectNameLineEdit_->setPalette(inputPalette_);
        }

        errorMessageLabel_->setText(projectNameErrorMessage_ + projectParentDirectoryErrorMessage_);
        createButton_->setEnabled(isProjectNameValid_ && isProjectParentDirectoryValid_);
        projectNameIsDirty_ = true;
    }

    void NewProjectDialog::validateProjectParentDirectoryInput() {
        const QString path = projectParentDirectoryLineEdit_->text();
        const QFileInfo pathInfo(path);
        isProjectParentDirectoryValid_ = false;
        projectParentDirectoryErrorMessage_.clear();

        if (path.isEmpty()) {
            projectParentDirectoryErrorMessage_ += tr("Project Path cannot be empty.\n");
        } else if (!pathInfo.exists()) {
            projectParentDirectoryErrorMessage_ += tr("Project Path does not exist.\n");
        } else if (!pathInfo.isDir()) {
            projectParentDirectoryErrorMessage_ += tr("Project Path is not a directory.\n");
        } else if (!pathInfo.isWritable()) {
            projectParentDirectoryErrorMessage_ += tr("Project Path is not writable.\n");
        } else {
            isProjectParentDirectoryValid_ = true;
        }

        if (!isProjectParentDirectoryValid_ && projectParentDirectoryIsDirty_) {
            projectParentDirectoryLineEdit_->setPalette(invalidPalette_);
        } else {
            projectParentDirectoryLineEdit_->setPalette(inputPalette_);
        }

        errorMessageLabel_->setText(projectNameErrorMessage_ + projectParentDirectoryErrorMessage_);
        createButton_->setEnabled(isProjectNameValid_ && isProjectParentDirectoryValid_);
        projectParentDirectoryIsDirty_ = true;
    }


    void NewProjectDialog::onBrowsePathClicked() {
        QString initialPath = projectParentDirectoryLineEdit_->text();
        if (initialPath.isEmpty() || !QFileInfo(initialPath).isDir()) {
            initialPath = QDir::homePath();
        }

        const QString directory = QFileDialog::getExistingDirectory(
            this,
            tr("Select Project Directory"),
            initialPath,
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        if (!directory.isEmpty()) {
            projectParentDirectoryLineEdit_->setText(directory);
        }
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

    QString NewProjectDialog::getProjectParentDirectory() const {
        return projectParentDirectoryLineEdit_->text();
    }
}
