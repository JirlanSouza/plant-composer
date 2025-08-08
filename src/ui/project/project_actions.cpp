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

#include "project_actions.h"

#include "project_properties_dialog.h"

namespace project {
    ProjectActions::ProjectActions(ProjectViewModel *viewModel, QWidget *parent)
        : QObject(parent), viewModel_(viewModel), parent_(parent) {
    }

    bool ProjectActions::canEditProperties() const {
        return viewModel_->hasOpenedProject();
    }

    void ProjectActions::editProperties() const {
        if (!canEditProperties()) {
            return;
        }

        ProjectPropertiesDialog dialog(viewModel_->getProject(), parent_);

        if (dialog.exec() == QDialog::Accepted) {
            const QString newName = dialog.getName();
            const QString newDescription = dialog.getDescription();

            if (newName.toStdString() != viewModel_->getProject()->getName()) {
                viewModel_->renameProject(newName.toStdString());
            }

            if (newDescription.toStdString() != viewModel_->getProject()->getDescription()) {
                viewModel_->changeProjectComment(newDescription.toStdString());
            }
        }
    }
}
