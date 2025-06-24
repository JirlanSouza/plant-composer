#pragma once
#include <QDialog>
#include <QGroupBox>
#include <QTableWidget>
#include "../../../domain/components_library/model/component.h"

namespace dcl = domain::components_library;

namespace ui::components_library {
    class LibraryComponentPropertiesView final : public QDialog {
        Q_OBJECT

    public:
        LibraryComponentPropertiesView(QWidget *parent, const dcl::Component &component,
                                       std::unordered_map<int, dcl::PortType> &portTypes);

        ~LibraryComponentPropertiesView() override = default;

    private:
        const dcl::Component &component_;
        std::unordered_map<int, dcl::PortType> &portTypes_;

        void buildUI();

        static QLayout *buildGroupLayout(QLayout *parentLayout, const std::string &groupName);

        static QTableWidget *buildTable(QLayout *parentLayout, int tableRows, const QStringList &headers);

        void buildConstantsView(QLayout *layout) const;

        void buildPortsView(QLayout *mainLayout) const;

        void buildSignalsView(QLayout *layout) const;

        static QTableWidgetItem *newTableItem(const std::string &strValue);
    };
}
