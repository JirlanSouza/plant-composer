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

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>

namespace project {
    class NewProjectDialog : public QDialog {
        Q_OBJECT

    public:
        explicit NewProjectDialog(QWidget *parent = nullptr);

        ~NewProjectDialog() override = default;

        [[nodiscard]] QString getProjectName() const;

        [[nodiscard]] QString getProjectDescription() const;

        [[nodiscard]] QString getProjectAuthor() const;

        [[nodiscard]] QString getProjectParentDirectory() const;

    private:
        QLineEdit *projectNameLineEdit_;
        QTextEdit *projectDescriptionTextEdit_;
        QLineEdit *projectAuthorLineEdit_;
        QLineEdit *projectParentDirectoryLineEdit_;
        QPushButton *createButton_;
        QPushButton *cancelButton_;
        QPushButton *browsePathButton_;
        QLabel *errorMessageLabel_;
        QPalette inputPalette_;
        QPalette invalidPalette_;
        bool isProjectNameValid_;
        bool projectNameIsDirty_;
        bool isProjectParentDirectoryValid_;
        bool projectParentDirectoryIsDirty_;
        QString projectNameErrorMessage_;
        QString projectParentDirectoryErrorMessage_;

        void setupUi();

        void setupConnections();

    private slots:
        void onBrowsePathClicked();

        void validateProjectNameInput();

        void validateProjectParentDirectoryInput();
    };
}
