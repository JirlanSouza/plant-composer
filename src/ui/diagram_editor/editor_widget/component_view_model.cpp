#include "component_view_model.h"

namespace ui::diagram_editor {
    ComponentViewModel::ComponentViewModel(dd::ComponentInstance *componentInstance, QObject *parent)
        : QObject(parent), componentInstance_(componentInstance) {
    }

    QString ComponentViewModel::getId() const {
        return QString::fromStdString(componentInstance_->getId());
    }

    QString ComponentViewModel::getName() const {
        return QString::fromStdString(componentInstance_->getName());
    }

    QString ComponentViewModel::getSymbolPath() const {
        return QString::fromStdString(componentInstance_->getSymbolPath());
    }

    QPointF ComponentViewModel::getPosition() const {
        const auto &pos = componentInstance_->getPosition();
        return QPointF(pos.x, pos.y);
    }

    void ComponentViewModel::setPosition(const QPointF &newPosition) {
        auto pos = componentInstance_->getPosition();
        pos.x = static_cast<int>(newPosition.x());
        pos.y = static_cast<int>(newPosition.y());
        componentInstance_->setPosition(pos);

        emit positionChanged();
    }
}
