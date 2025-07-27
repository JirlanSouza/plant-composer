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

#include "side_bar.h"

#include <QMainWindow>
#include <qscrollarea.h>
#include <QVBoxLayout>
#include <QWindow>

namespace app_layout {
    SideBar::SideBar(QMainWindow *parent, const Side side): QDockWidget(parent),
        side_(side),
        stacked_(new QStackedWidget(this)) {
        auto palette = QPalette();
        palette.setColor(QPalette::Window, palette.base().color());
        stacked_->setPalette(palette);
        stacked_->setAutoFillBackground(true);
        stacked_->setMinimumWidth(32);
        setWidget(stacked_);
    }

    SideBar::~SideBar() = default;


    void SideBar::addItem(const QString &label, QWidget *content) const {
        stacked_->addWidget(content);

        if (stacked_->count() == 1) {
            stacked_->setCurrentIndex(0);
            stacked_->show();
        }
    }

    int SideBar::getBeforeCollapsedWidth() const {
        return lastExpandedWidth_;
    }


    void SideBar::onItemChanged(const int index) {
        if (index < 0 || index >= stacked_->count()) {
            return;
        }


        if (collapsed_ && stacked_->isHidden()) {
            stacked_->setCurrentIndex(index);
            stacked_->show();
            show();
            collapsed_ = false;
            emit collapsedChange(collapsed_, side_);
            return;
        }

        if (stacked_->currentIndex() == index) {
            lastExpandedWidth_ = width();
            stacked_->hide();
            hide();
            collapsed_ = true;
            resize(tabBarWidth_, size().height());
            updateGeometry();
            emit collapsedChange(collapsed_, side_);
            return;
        }

        stacked_->setCurrentIndex(index);
        stacked_->show();
        show();
        collapsed_ = false;
        emit collapsedChange(collapsed_, side_);
    }
}
