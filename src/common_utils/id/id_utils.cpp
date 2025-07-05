#include "id_utils.h"

#include <QUuid>

std::string newId() {
    const auto uuid = new QUuid();
    return uuid->toString(QUuid::WithoutBraces).toStdString();
}
