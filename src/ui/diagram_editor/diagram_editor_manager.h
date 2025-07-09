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

#include <qtabwidget.h>

#include "domain/settings/app_settings.h"
#include "diagram_manager.h"
#include "new_diagram_dialog.h"
#include "editor_widget/diagram_editor_scene.h"
#include "editor_widget/diagram_editor_view.h"
#include "editor_widget/diagram_view_model.h"
#include "ui/project/project_view_model.h"

namespace dcl = domain::components_library;
namespace dst = domain::settings;

namespace ui::diagram_editor {
    struct DiagramEditorTab {
        DiagramViewModel *diagram;
        DiagramEditorView *diagramEditorView_;
        DiagramEditorScene *diagramEditorScene_;
    };

    class DiagramEditorManager : public QObject {
        Q_OBJECT

    public:
        explicit DiagramEditorManager(
            DiagramManager *diagramManager,
            ui::project::ProjectViewModel *projectViewModel,
            QWidget *parent = nullptr
        );

        ~DiagramEditorManager() override;

        [[nodiscard]] QWidget *getView() const;

    public slots:
        void addNewDiagram() const;

        void onInvalidDiagramName(const std::string &diagramName) const;

    private:
        DiagramManager *diagramManager_;
        ui::project::ProjectViewModel *projectViewModel_;
        QTabWidget *editorArea_;
        std::unordered_map<std::string, DiagramEditorTab> diagramEditorTabs_;
        std::vector<std::string> editorTabsOrder_;
        NewDiagramDialog *newDiagramDialog_;

    private slots:
        void onOpenedDiagram(const std::string &diagramId);

        void onTabMoved(int from, int to);

        void onTabCloseRequested(int index);
    };
}
