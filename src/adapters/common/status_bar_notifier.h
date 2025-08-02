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

#pragma once

#include <QStatusBar>
#include <QObject>
#include <QMessageBox>

#include "domain/common/iuser_notifier.h"

namespace common {
    class StatusBarNotifier : public QObject, public IUserNotifier {
        Q_OBJECT

    public:
        explicit StatusBarNotifier();

        void setStatusBar(QStatusBar *statusBar);

        void showToast(const std::string &message, NotificationLevel level) override;

        void showAlert(const std::string &title, const std::string &message, NotificationLevel level) override;

    private:
        QStatusBar *statusBar_;
        QWidget *parentWidget_;

        static QMessageBox::Icon getIconForLevel(NotificationLevel level) ;
    };
}
