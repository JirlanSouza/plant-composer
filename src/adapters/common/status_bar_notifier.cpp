/*
 * plant-composer
 * Copyright (c) 2025 jirlansouza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "status_bar_notifier.h"
#include <QMessageBox>
#include <QTimer>

namespace common {
    StatusBarNotifier::StatusBarNotifier() : QObject(),
        statusBar_(nullptr), parentWidget_(nullptr) {
    }

    void StatusBarNotifier::setStatusBar(QStatusBar *statusBar) {
        statusBar_ = statusBar;
    }

    void StatusBarNotifier::showToast(const std::string &message, NotificationLevel level) {
        statusBar_->showMessage(QString::fromStdString(message), 5000);
    }

    void StatusBarNotifier::showAlert(const std::string &title, const std::string &message, NotificationLevel level) {
        auto *messageBox = new QMessageBox(
            getIconForLevel(level),
            QString::fromStdString(title),
            QString::fromStdString(message),
            QMessageBox::Ok,
            parentWidget_
        );
        messageBox->setAttribute(Qt::WA_DeleteOnClose);
        messageBox->show();
    }

    QMessageBox::Icon StatusBarNotifier::getIconForLevel(const NotificationLevel level) {
        switch (level) {
            case NotificationLevel::Info:
                return QMessageBox::Information;
            case NotificationLevel::Warning:
                return QMessageBox::Warning;
            case NotificationLevel::Error:
                return QMessageBox::Critical;
            default:
                return QMessageBox::NoIcon;
        }
    }
}
