#include "main_window.h"

#include "./ui_main_window.h"
#include "../components_library/components_library_model.h"
#include "components_library/components_library_view.h"

class LibraryComponentPropertiesView;

MainWindow::MainWindow(QWidget *parent, const std::vector<Library> &libraries, AppSettings *appSettings)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow),
      appSettings_(appSettings),
      libraries_(libraries) {
    ui->setupUi(this);
    libraryModel_ = new ComponentsLibraryModel(this, libraries, appSettings->getComponentIconPath());
    libraryView_ = new ComponentsLibraryView(this, libraryModel_);

    ui->rightDock->setWidget(libraryView_);

    connect(libraryView_, &ComponentsLibraryView::componentPropertiesViewRequested, this,
            &MainWindow::showComponentLibraryPropertiesDialog);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showComponentLibraryPropertiesDialog(const int libraryId, const int componentId) {
    const Library library = libraries_[libraryId];
    const Component component = library.components[componentId];
    const auto dialog = new LibraryComponentPropertiesView(this, component);
    dialog->exec();
}
