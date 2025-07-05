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

#include "editor_widget/diagram_view_model.h"
#include "domain/components_library/model/library.h"
#include "domain/settings/app_settings.h"

namespace dd = domain::diagram;
namespace dcl = domain::components_library;
namespace dst = domain::settings;

namespace ui::diagram_editor {
    class DiagramManager final : public QObject {
        Q_OBJECT

    public:
        explicit DiagramManager(std::vector<dcl::Library> *libraries, dst::AppSettings *appSettings, QObject *parent);

        ~DiagramManager() override;

        void openDiagram(const std::string &diagramId);


        DiagramViewModel *getDiagram(const std::string &diagramId);

    signals:
        void diagramOpened(std::string diagramId);

    private:
        std::vector<dcl::Library> *libraries_;

        std::unordered_map<std::string, DiagramViewModel *> openedDiagrams_;
        dst::AppSettings *appSettings_;

        [[nodiscard]] const dcl::Component *getComponent(int libraryId, int componentId) const;

    public slots:
        void addComponent(const std::string &diagramId, int libraryId, int componentId, QPointF posi) const;
    };
}
