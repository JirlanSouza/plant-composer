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

#include "mime_types.h"

namespace project {
    ProjectTreeView::ProjectTreeView(
        common::ILoggerFactory *loggerFactory,
        ProjectTreeModel *model,
        QWidget *parent
    ): QTreeView(parent), logger_(loggerFactory->getLogger("ProjectTreeView")) {
        QTreeView::setModel(model);
        setHeaderHidden(true);
        setExpandsOnDoubleClick(true);
        setSelectionMode(SingleSelection);
        setLayoutDirection(Qt::LeftToRight);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setEditTriggers(QAbstractItemView::NoEditTriggers);

        setDragEnabled(true);
        setAcceptDrops(true);
        setDropIndicatorShown(true);
        setDragDropMode(QAbstractItemView::InternalMove);

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

    void ProjectTreeView::dragMoveEvent(QDragMoveEvent *event) {
        logger_->info("Drag move event");
        const QModelIndex index = indexAt(event->position().toPoint());

        if (!index.isValid()) {
            logger_->info("Ignore drag move event for invalid index");
            event->ignore();
        }

        logger_->info("Valid index at drag move event: {}", index.row());
        const std::optional<ProjectNodeItem *> itemOpt = dynamic_cast<ProjectTreeModel *>(model())->
                itemFromIndex(index);

        if (!itemOpt.has_value()) {
            logger_->info("Ignore drag move event for invalid item");
            event->ignore();
            return;
        }

        if (itemOpt.value()->isDropEnabled()) {
            logger_->info("Accepted drag move event for folder or file");
            QTreeView::dragMoveEvent(event);
            return;
        }

        logger_->info("Ignore drag move event for non-folder or non-file item");
        event->ignore();
    }

    void ProjectTreeView::dropEvent(QDropEvent *event) {
        logger_->info("Drop event started");
        if (!event->mimeData()->hasFormat(MIME_TYPE_INTERNAL_PROJECT_NODE)) {
            logger_->info("Ignore drag move event not have mime format: {}", MIME_TYPE_INTERNAL_PROJECT_NODE);
            event->ignore();
            return;
        }

        const QModelIndex index = indexAt(event->position().toPoint());
        if (!index.isValid()) {
            logger_->info("Ignore drag move event for invalid index: {}", index.row());
            event->ignore();
            return;
        }

        const auto itemOpt = dynamic_cast<ProjectTreeModel *>(model())->itemFromIndex(index);
        if (!itemOpt.has_value()) {
            logger_->info("Ignore drag move event for invalid item");
            event->ignore();
            return;
        }

        const std::optional<ProjectContext> targetContextOpt = itemOpt.value()->getContext();
        if (!targetContextOpt.has_value()) {
            logger_->info("Ignore drag move event for invalid context item");
            event->ignore();
            return;
        }

        emit internalNodeDropped(event->mimeData(), targetContextOpt.value());
        event->acceptProposedAction();
    }
}
