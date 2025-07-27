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

#include "side_bar_tabs.h"

#include <QVBoxLayout>

#include "side_bar_item.h"

namespace app_layout {
    SideBarTabs::SideBarTabs(const int width, QWidget *parent): QToolBar(parent), layout_(new QVBoxLayout(this)) {
        setFixedWidth(width);
        layout_->setContentsMargins(0, 0, 0, 0);
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignTop);

        setMovable(false);
        setFloatable(false);
        setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
        setOrientation(Qt::Vertical);
    }

    SideBarTabs::~SideBarTabs() = default;

    int SideBarTabs::addTab(const QString &label) {
        const int index = itemsCount_++;
        const auto item = new SideBarItem(index, label, this);
        addWidget(item);

        connect(item, &SideBarItem::clicked, this, &SideBarTabs::onTabClicked);
        return index;
    }

    void SideBarTabs::onTabClicked(const int index) {
        currentIndex_ = index;
        emit indexChanged(index);
    }

    int SideBarTabs::tabsCount() const {
        return itemsCount_;
    }

    int SideBarTabs::currentTabIndex() const {
        return currentIndex_;
    }

    void SideBarTabs::setCurrentTabIndex(const int index) {
        currentIndex_ = index;
    }
}
