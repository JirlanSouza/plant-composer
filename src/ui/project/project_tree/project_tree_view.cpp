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

#include "project_tree_view.h"

namespace project {
    ProjectTreeView::ProjectTreeView(ProjectTreeModel *model, QWidget *parent): QTreeView(parent) {
        QTreeView::setModel(model);
        setHeaderHidden(true);
        setExpandsOnDoubleClick(true);
        setSelectionMode(SingleSelection);
        setLayoutDirection(Qt::LeftToRight);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setEditTriggers(QAbstractItemView::NoEditTriggers);

        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
        setDefaultDropAction(Qt::CopyAction);

        setRootIsDecorated(false);
        expandAll();
    }

    void ProjectTreeView::toggleExpanded(const QModelIndex &index) {
        if (index.isValid()) {
            const bool expanded = isExpanded(index);
            setExpanded(index, !expanded);
        }
    }

    void ProjectTreeView::keyPressEvent(QKeyEvent *event) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            QModelIndex index = currentIndex();
            if (index.isValid()) {
                if (state() == QAbstractItemView::EditingState) {
                    event->accept();
                    return;
                }

                if (model()->hasChildren(index)) {
                    toggleExpanded(index);
                    return;
                }

                emit activated(index);
            }
        } else {
            QTreeView::keyPressEvent(event);
        }
    }
}
