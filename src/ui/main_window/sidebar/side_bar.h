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
#include <QDockWidget>
#include <QStackedWidget>

namespace ui::layout {
    enum Side {
        LEFT = 0, RIGHT,
    };

    class SideBar : public QDockWidget {
        Q_OBJECT

    public:
        SideBar(QMainWindow *parent = nullptr, Side side = LEFT);

        ~SideBar() override;

        void addItem(const QString &label, QWidget *content) const;

        int getBeforeCollapsedWidth() const;

    signals:
        void collapsedChange(bool collapsed, Side side);

    public slots:
        void onItemChanged(int index);

    private:
        Side side_;
        QStackedWidget *stacked_;

        int tabBarWidth_ = 32;
        int lastExpandedWidth_ = 220;
        bool collapsed_ = false;
    };
}
