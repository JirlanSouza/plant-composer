#pragma once
#include <QMimeData>
#include <QTreeView>

#include "components_library_model.h"
#include "ui/components_library/properties_dialog/component_properties_view.h"

namespace ui::components_library {
    class ComponentsLibraryView final : public QTreeView {
        Q_OBJECT

    public:
        explicit ComponentsLibraryView(QWidget *parent = nullptr, ComponentsLibraryModel *model = nullptr);

    signals:
        void componentPropertiesViewRequested(int libraryId, int componentId);

    private:
        ComponentsLibraryModel *model_;
        LibraryComponentPropertiesView *propertiesView_{};

        void setupContextMenu();
    };
}
