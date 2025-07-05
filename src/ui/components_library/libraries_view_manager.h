#pragma once
#include "component_tree/components_library_model.h"
#include "component_tree/components_library_view.h"
#include "domain/settings/app_settings.h"

namespace dcl = domain::components_library;
namespace dst = domain::settings;

namespace ui::components_library {
    class LibrariesViewManager : public QObject {
        Q_OBJECT

    public:
        explicit LibrariesViewManager(
            std::vector<dcl::Library> *libraries,
            dst::AppSettings *appSettings,
            QWidget *parent = nullptr
        );

        ~LibrariesViewManager() override;

        [[nodiscard]] QWidget *getView() const;

    private:
        ComponentsLibraryModel *libraryModel_;
        ComponentsLibraryView *libraryView_;
        std::vector<dcl::Library> *libraries_;
    };
}
