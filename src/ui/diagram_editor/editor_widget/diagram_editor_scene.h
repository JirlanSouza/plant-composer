/*
 * plant-composer
 * Copyright (C) 2025 Jirlan Souza
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
#include <QGraphicsScene>
#include <QPainter>

#include "diagram_view_model.h"
#include "component_instance_view.h"
#include "component_view_model.h"

namespace ui::diagram_editor {
    class DiagramEditorScene final : public QGraphicsScene {
        Q_OBJECT

    public:
        explicit DiagramEditorScene(DiagramViewModel *diagramModel, QObject *parent = nullptr);

        ~DiagramEditorScene() override = default;

    protected:
        void drawBackground(QPainter *painter, const QRectF &rect) override;

    private:
        DiagramViewModel *diagramModel_;
        QObject *parent_;
        const int gridSize = 50;
        const QColor gridColor = QColor(220, 220, 220);
        std::unordered_map<std::string, ComponentInstanceView *> componentInstanceViews_;

    public slots:
        void onComponentAdded(ui::diagram_editor::ComponentViewModel *componentViewModel);
    };
}
