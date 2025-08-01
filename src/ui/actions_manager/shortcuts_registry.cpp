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

#include "shortcuts_registry.h"

namespace app_actions {
    ShortcutRegistry::ShortcutRegistry(QObject *parent): QObject(parent) {
        shortcuts_[ShortcutId::NewProject] = QKeySequence("Ctrl+N");
        shortcuts_[ShortcutId::OpenProject] = QKeySequence("Ctrl+O");
        shortcuts_[ShortcutId::SaveProject] = QKeySequence("Ctrl+S");
        shortcuts_[ShortcutId::CloseProject] = QKeySequence("Ctrl+W");
        shortcuts_[ShortcutId::NewFile] = QKeySequence("Ctrl+Shift+N");
        shortcuts_[ShortcutId::NewFolder] = QKeySequence("Ctrl+Shift+F");
        shortcuts_[ShortcutId::Copy] = QKeySequence::Copy;
        shortcuts_[ShortcutId::Cut] = QKeySequence::Cut;
        shortcuts_[ShortcutId::Paste] = QKeySequence::Paste;
        shortcuts_[ShortcutId::Rename] = QKeySequence("F2");
        shortcuts_[ShortcutId::Delete] = QKeySequence::Delete;
    }

    QKeySequence ShortcutRegistry::getShortcut(const ShortcutId id) const {
        return shortcuts_.value(id, QKeySequence());
    }

    void ShortcutRegistry::setShortcut(const ShortcutId id, const QKeySequence &sequence) {
        shortcuts_[id] = sequence;
    }
}
