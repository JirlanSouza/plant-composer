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

#include "actions_manager.h"

namespace app_actions {
    ActionsManager::ActionsManager(ShortcutRegistry *shortcutRegistry, QObject *parent) : QObject(parent),
        shortcutRegistry_(shortcutRegistry) {
    }

    void ActionsManager::addAction(
        const ActionGroupType group,
        QAction *action,
        const std::optional<ShortcutId> shortcutId
    ) {
        if (shortcutRegistry_ && action && shortcutId.has_value()) {
            if (const QKeySequence shortcut = shortcutRegistry_->getShortcut(shortcutId.value()); !shortcut.isEmpty()) {
                action->setShortcut(shortcut);
            }
        }
        actions_[group].append(action);
    }

    QList<QAction *> ActionsManager::getActions(const ActionGroupType group) const {
        return actions_.value(group);
    }
}
