#pragma once
#include <QObject>
#include <QPointF>
#include <QString>

#include "domain/diagram/model/component_instance.h"

namespace dd = domain::diagram;

namespace ui::diagram_editor {
    class ComponentViewModel : public QObject {
        Q_OBJECT

        Q_PROPERTY(QString id READ getId CONSTANT)
        Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
        Q_PROPERTY(QString symbolPath READ getSymbolPath CONSTANT)
        Q_PROPERTY(QPointF position READ getPosition WRITE setPosition NOTIFY positionChanged)

    public:
        explicit ComponentViewModel(dd::ComponentInstance *componentInstance, QObject *parent = nullptr);

        ~ComponentViewModel() override = default;

        [[nodiscard]] QString getId() const;

        [[nodiscard]] QString getName() const;

        [[nodiscard]] QString getSymbolPath() const;

        [[nodiscard]] QPointF getPosition() const;

        void setPosition(const QPointF &newPosition);

        [[nodiscard]] dd::ComponentInstance *getDomainComponent() const { return componentInstance_; }

    signals:
        void nameChanged();

        void positionChanged();

    private:
        dd::ComponentInstance *componentInstance_;
    };
}
