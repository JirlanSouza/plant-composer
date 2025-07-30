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

#include "domain/project/model/project_node.h"

namespace project {
    class Clipboard {
    public:
        enum class Mode { NONE, COPY, CUT };

        Clipboard() = default;

        void setCopy(const project::ProjectNode *node) {
            node_ = node;
            mode_ = Mode::COPY;
        }

        void setCut(const project::ProjectNode *node) {
            node_ = node;
            mode_ = Mode::CUT;
        }

        void clear() {
            node_ = nullptr;
            mode_ = Mode::NONE;
        }

        [[nodiscard]] bool isEmpty() const {
            return mode_ == Mode::NONE || node_ == nullptr;
        }

        [[nodiscard]] bool isCopy() const {
            return mode_ == Mode::COPY && node_ != nullptr;
        }

        [[nodiscard]] bool isCut() const {
            return mode_ == Mode::CUT && node_ != nullptr;
        }

        [[nodiscard]] const project::ProjectNode *getNode() const {
            return node_;
        }

    private:
        const project::ProjectNode *node_ = nullptr;
        Mode mode_ = Mode::NONE;
    };
}
