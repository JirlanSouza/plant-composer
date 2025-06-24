#pragma once
#include <QMainWindow>
#include "../../domain/components_library/model/library.h"
#include "../../domain/settings/app_settings.h"
#include "../components_library/components_library_model.h"
#include "chart_editor/chart_editor_scene.h"
#include "chart_editor/chart_editor_view.h"
#include "chart_editor/chart_interface.h"
#include "components_library/components_library_view.h"

namespace dcl = domain::components_library;
namespace dst = domain::settings;
namespace ucl = ui::components_library;
namespace uce = ui::chart_editor;

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

namespace ui::main_window {
    class AppMainWindow final : public QMainWindow {
        Q_OBJECT

    public:
        explicit AppMainWindow(QWidget *parent, const std::shared_ptr<std::vector<dcl::Library> > &libraries,
                               std::shared_ptr<dst::AppSettings> appSettings);

        ~AppMainWindow() override;

    private:
        Ui::MainWindow *ui;
        std::shared_ptr<dst::AppSettings> appSettings_;
        ucl::ComponentsLibraryModel *libraryModel_;
        ucl::ComponentsLibraryView *libraryView_;
        const std::shared_ptr<std::vector<dcl::Library> > libraries_;
        uce::ChartInterface *chartInterface_;
        uce::ChartEditorView *chartEditorView_;
        uce::ChartEditorScene *chartEditorScene_;

    private slots:
        void showComponentLibraryPropertiesDialog(int libraryId, int componentId);
    };
}
