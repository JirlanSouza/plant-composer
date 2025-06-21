#pragma once

#include <QTreeView>
#include "components_library_model.h"
#include "properties_dialog/library_component_properties_view.h"

class ComponentsLibraryView final : public QTreeView {
    Q_OBJECT

public:
    explicit ComponentsLibraryView(QWidget *parent = nullptr, ComponentsLibraryModel *model = nullptr);

signals:
    void componentPropertiesViewRequested(int libraryId, int componentId);

private:
    ComponentsLibraryModel *model_;
    LibraryComponentPropertiesView *propertiesView_;

    void setupContextMenu();
};
