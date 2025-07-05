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

#include "components_library_view.h"

#include <qevent.h>
#include <QMenu>

namespace ui::components_library {
    ComponentsLibraryView::ComponentsLibraryView(QWidget *parent, ComponentsLibraryModel *model): QTreeView(parent),
        model_(model) {
        QTreeView::setModel(model_);
        setHeaderHidden(true);
        setExpandsOnDoubleClick(true);
        setSelectionMode(SingleSelection);
        setLayoutDirection(Qt::LeftToRight);
        setContextMenuPolicy(Qt::CustomContextMenu);

        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
        setDefaultDropAction(Qt::CopyAction);

        setupContextMenu();
    }

    void ComponentsLibraryView::setupContextMenu() {
        connect(
            this,
            &QTreeView::customContextMenuRequested,
            [&](const QPoint &pos) {
                const QModelIndex index = indexAt(pos);

                if (!index.isValid() || !index.parent().isValid()) return;

                QMenu menu;
                menu.addAction(
                    tr("Add to chart"),
                    [&] {
                        edit(index);
                    }
                );

                menu.addAction(
                    tr("Properties"),
                    [&] {
                        emit componentPropertiesViewRequested(index.parent().row(), index.row());
                    }
                );

                menu.exec(viewport()->mapToGlobal(pos));
            }
        );
    }
}
