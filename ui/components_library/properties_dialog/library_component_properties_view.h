#pragma once
#include <QDialog>
#include <QTableWidget>

#include "../../../domain/components_library/model/component.h"

class LibraryComponentPropertiesView final : public QDialog {
    Q_OBJECT

public:
    LibraryComponentPropertiesView(QWidget *parent, const Component &component);

    ~LibraryComponentPropertiesView() override = default;

private:
    const Component &component_;

    void buildUI();

    static QTableWidget *buildPropertyView(QLayout *mainLayout, const std::string &groupName,
                                           int tableRows, int tableColumns);

    void buildConstantsView(QLayout *layout) const;

    void buildInputsView(QLayout *layout) const;

    void buildOutputsView(QLayout *layout) const;
};
