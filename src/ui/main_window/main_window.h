#pragma once
#include "app_layout_manager.h"
#include "domain/components_library/model/library.h"
#include "domain/settings/app_settings.h"
#include "components_library/libraries_view_manager.h"
#include "diagram_editor/diagram_editor_manager.h"

namespace dcl = domain::components_library;
namespace dst = domain::settings;
namespace ucl = ui::components_library;
namespace ude = ui::diagram_editor;
namespace uil = ui::layout;

namespace ui::main_window {
    class AppMainWindow final : public QMainWindow {
        Q_OBJECT

    public:
        AppMainWindow(
            std::vector<dcl::Library> *libraries,
            dst::AppSettings *appSettings,
            QWidget *parent
        );

        ~AppMainWindow() override;

    private:
        std::shared_ptr<dst::AppSettings> appSettings_;
        uil::AppLayoutManager *appLayoutManager_;
        ucl::LibrariesViewManager *librariesViewManager_;
        ude::DiagramEditorManager *diagramEditorManager_;

    private slots:
        void showComponentLibraryPropertiesDialog(int libraryId, int componentId);
    };
}
