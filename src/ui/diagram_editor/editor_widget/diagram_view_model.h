#pragma once
#include <QObject>
#include <QList>

#include "domain/diagram/model/diagram.h"
#include "component_view_model.h"

namespace dd = domain::diagram;

namespace ui::diagram_editor {
    class DiagramViewModel : public QObject {
        Q_OBJECT

    public:
        explicit DiagramViewModel(dd::Diagram *diagram, QObject *parent = nullptr);

        ~DiagramViewModel() override;

        void addComponent(const dd::ComponentInstance &component);

        ComponentViewModel *getLastAddedComponentViewModel();

        [[nodiscard]] int componentsCount() const;

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

    signals:
        void componentAdded(ui::diagram_editor::ComponentViewModel *componentViewModel);

    private:
        dd::Diagram *diagram_;

        QList<ComponentViewModel *> componentViewModels_;
    };
}
