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

#include <qkeysequence.h>
#include <QMap>
#include <QObject>

namespace app_actions {
    enum class ShortcutId {
        NewProject,
        OpenProject,
        SaveProject,
        CloseProject,
        NewFile,
        NewFolder,
        Copy,
        Cut,
        Paste,
        Rename,
        Delete,
    };

    class ShortcutRegistry : public QObject {
    public:
        explicit ShortcutRegistry(QObject *parent = nullptr);

        [[nodiscard]] QKeySequence getShortcut(ShortcutId id) const;

        void setShortcut(ShortcutId id, const QKeySequence &sequence);

    private:
        QMap<ShortcutId, QKeySequence> shortcuts_;
    };
}
