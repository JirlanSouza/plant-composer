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

/*
 * plant-composer
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
#include <QToolBar>

namespace app_layout {
    class SideBarTabs : public QToolBar {
        Q_OBJECT

    public:
        SideBarTabs(int width = 32, QWidget *parent = nullptr);

        ~SideBarTabs() override;

        int addTab(const QString &label);

        [[nodiscard]] int tabsCount() const;

        [[nodiscard]] int currentTabIndex() const;

        void setCurrentTabIndex(int index);

    signals:
        void indexChanged(int index);

    public slots:
        void onTabClicked(int index);

    private:
        QLayout *layout_;
        int itemsCount_ = 0;
        int currentIndex_ = 0;
    };
}
