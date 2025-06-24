#include "components_library_view.h"

#include <qevent.h>
#include <QMenu>

namespace ui::components_library {
    ComponentsLibraryView::ComponentsLibraryView(QWidget *parent, ComponentsLibraryModel *model): QTreeView(parent),
        model_(model) {
        QTreeView::setModel(model_);
        setHeaderHidden(false);
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
        connect(this, &QTreeView::customContextMenuRequested,
                [&](const QPoint &pos) {
                    const QModelIndex index = indexAt(pos);

                    if (!index.isValid() || !index.parent().isValid()) return;

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
}
