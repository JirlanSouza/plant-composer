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

#include <QObject>
#include <QWidget>

#include "project_view_model.h"

namespace project {
    class ProjectActions : public QObject {
    public:
        explicit ProjectActions(ProjectViewModel *viewModel, QWidget *parent = nullptr);

        [[nodiscard]] bool canEditProperties() const;

    public slots:
        void editProperties() const;

    signals:
        void canEditPropertiesChanged();

    private:
        ProjectViewModel *viewModel_;
        QWidget *parent_;
    };
}
