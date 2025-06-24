#include "main_window.h"

#include <iostream>
#include <__ostream/basic_ostream.h>

#include "ui_main_window.h"
#include "components_library/components_library_model.h"
#include "chart_editor/chart_editor_scene.h"
#include "chart_editor/chart_editor_view.h"
#include "components_library/components_library_view.h"

namespace ui::main_window {
    AppMainWindow::AppMainWindow(QWidget *parent, const std::shared_ptr<std::vector<dcl::Library> > &libraries,
                                 std::shared_ptr<dst::AppSettings> appSettings)
        : QMainWindow(parent)
          , ui(new Ui::MainWindow),
          appSettings_(appSettings),
          libraries_(libraries) {
        ui->setupUi(this);
        libraryModel_ = new ucl::ComponentsLibraryModel(this, *libraries.get(), appSettings->getComponentIconPath());
        libraryView_ = new ucl::ComponentsLibraryView(this, libraryModel_);
        appSettings->getAssetsDir();
        appSettings_->getAssetsDir();

        ui->rightDock->setWidget(libraryView_);

        auto chart = std::make_shared<dce::Chart>("dfjdkfj9j3148r", "Chart_001");

        chartInterface_ = new uce::ChartInterface(this, libraries_, chart, appSettings_);

        chartEditorView_ = new uce::ChartEditorView(this);
        chartEditorScene_ = new uce::ChartEditorScene(chartEditorView_);
        chartEditorScene_->setSceneRect(-2000, -2000, 4000, 4000);
        chartEditorView_->setScene(chartEditorScene_);

        const auto centralLayout = new QVBoxLayout();
        centralLayout->addWidget(chartEditorView_);
        ui->centralwidget->setLayout(centralLayout);

        connect(libraryView_, &ucl::ComponentsLibraryView::componentPropertiesViewRequested, this,
                &AppMainWindow::showComponentLibraryPropertiesDialog);

        connect(chartEditorView_, &uce::ChartEditorView::addComponentToScene,
                chartInterface_,
                &uce::ChartInterface::addComponent);

        connect(chartInterface_, &uce::ChartInterface::componentAdded, chartEditorScene_,
                &uce::ChartEditorScene::onComponentAdded);
    }

    AppMainWindow::~AppMainWindow() {
        delete ui;
    }

    void AppMainWindow::showComponentLibraryPropertiesDialog(const int libraryId, const int componentId) {
        const dcl::Library library = libraries_->at(libraryId);
        const dcl::Component component = library.components[componentId];
        std::unordered_map<int, dcl::PortType> portTypes;

        for (const auto &portType: library.portTypes) {
            portTypes.emplace(portType.id, portType);
        }

        const auto dialog = new ucl::LibraryComponentPropertiesView(this, component, portTypes);
        dialog->exec();
    }
}
