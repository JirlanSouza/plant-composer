#include "libraries_view_manager.h"

namespace ui::components_library {
    LibrariesViewManager::LibrariesViewManager(
        std::vector<dcl::Library> *libraries,
        dst::AppSettings *appSettings,
        QWidget *parent
    ): QObject(parent), libraries_(libraries) {
        libraryModel_ = new ComponentsLibraryModel(parent, *libraries, appSettings->getComponentIconPath());
        libraryView_ = new ComponentsLibraryView(parent, libraryModel_);
    }

    LibrariesViewManager::~LibrariesViewManager() = default;

    QWidget *LibrariesViewManager::getView() const {
        return libraryView_;
    }
}
