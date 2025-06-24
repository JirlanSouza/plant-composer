#pragma once
#include <QStandardItemModel>
#include "../../domain/components_library/model/library.h"

namespace dcl = domain::components_library;

namespace ui::components_library {
    class ComponentsLibraryModel final : public QStandardItemModel {
    public:

        explicit ComponentsLibraryModel(QObject *parent, const std::vector<dcl::Library> &libraries,
                                        std::string componentIconPath);

        void addLibrary(int libraryIndex, const dcl::Library &library);

        QStringList mimeTypes() const override;

        QMimeData *mimeData(const QModelIndexList &indexes) const override;

    private:
        const std::vector<dcl::Library> &libraries_;
        const std::string componentIconPath_;

        [[nodiscard]] QIcon newComponentIcon() const {
            return QIcon(QString::fromStdString(componentIconPath_));
        }
    };
}
