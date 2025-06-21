#include "components_library_view.h"

#include <QMenu>

ComponentsLibraryView::ComponentsLibraryView(QWidget *parent, ComponentsLibraryModel *model): QTreeView(parent),
    model_(model) {
    setModel(model_);
    setHeaderHidden(false);
    setExpandsOnDoubleClick(true);
    setDragDropMode(InternalMove);
    setDragEnabled(true);
    setAcceptDrops(false);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(SingleSelection);
    setLayoutDirection(Qt::LeftToRight);
    setContextMenuPolicy(Qt::CustomContextMenu);

    setupContextMenu();
}

void ComponentsLibraryView::setupContextMenu() {
    connect(this, &QTreeView::customContextMenuRequested,
            [&](const QPoint &pos) {
                QModelIndex index = indexAt(pos);
                if (!index.isValid() || !index.parent().isValid())
                    return;

                QMenu menu;
                menu.addAction(tr("Add to chart"), [&] {
                    edit(index);
                });

                menu.addAction(tr("Properties"), [&] {
                    emit componentPropertiesViewRequested(index.parent().row(), index.row());
                });

                menu.exec(viewport()->mapToGlobal(pos));
            });
}
