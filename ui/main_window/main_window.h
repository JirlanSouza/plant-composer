#pragma once
#include <QMainWindow>
#include "../../domain/components_library/model/library.h"
#include "../../domain/settings/app_settings.h"
#include "../components_library/components_library_model.h"
#include "components_library/components_library_view.h"

class LibraryComponentPropertiesView;
QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, const std::vector<Library> &libraries, AppSettings *appSettings);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    AppSettings *appSettings_;
    ComponentsLibraryModel *libraryModel_;
    ComponentsLibraryView *libraryView_;

    const std::vector<Library> &libraries_;

private slots:
    void showComponentLibraryPropertiesDialog(int libraryId, int componentId);
};
