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

#include <QLabel>
#include <QMainWindow>
#include <qscrollarea.h>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWindow>

namespace app_layout {
    SideBar::SideBar(QMainWindow *parent, const Side side): QDockWidget(parent),
        side_(side),
        stacked_(new QStackedWidget(this)), itemsLabels_(new QList<QString>()) {
        auto palette = QPalette();
        palette.setColor(QPalette::Window, palette.alternateBase().color());
        stacked_->setPalette(palette);
        stacked_->setAutoFillBackground(true);
        stacked_->setMinimumWidth(32);

        title_ = new QLabel(this);
        collapseButton_ = new QToolButton(this);
        collapseButton_->setText(side == Side::LEFT ? "«" : "»");
        collapseButton_->setToolTip(tr("Collapse sidebar"));

        QColor hoverColor = QPalette().color(QPalette::Window);
        hoverColor.setAlpha(150);
        QString qss = QString(
            R"(
                QToolButton {
                    border: none;
                    background: transparent;
                    padding: 4px;
                }
                QToolButton:hover {
                    background-color: %1;
                    border-radius: 4px;
                }
            )"
        ).arg(hoverColor.lighter().name());

        collapseButton_->setStyleSheet(
            qss
        );
        connect(collapseButton_, &QToolButton::clicked, this, &SideBar::collapse);

        const auto titleBarLayout = new QHBoxLayout(this);

        if (side == Side::LEFT) {
            titleBarLayout->setContentsMargins(6, 0, 0, 0);
            titleBarLayout->addWidget(title_);
            titleBarLayout->addStretch();
            titleBarLayout->addWidget(collapseButton_);
        } else {
            titleBarLayout->setContentsMargins(0, 0, 0, 0);
            titleBarLayout->addWidget(collapseButton_);
            titleBarLayout->addWidget(title_);
        }

        const auto titleBarWidget = new QWidget(this);
        titleBarWidget->setLayout(titleBarLayout);
        setTitleBarWidget(titleBarWidget);

        setFeatures(QDockWidget::DockWidgetMovable);
        setWidget(stacked_);
    }

    SideBar::~SideBar() = default;


    void SideBar::addItem(const QString &label, QWidget *content) const {
        stacked_->addWidget(content);
        itemsLabels_->append(label);

        if (stacked_->count() == 1) {
            stacked_->setCurrentIndex(0);
            stacked_->show();
            title_->setText(label);
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
            title_->setText(itemsLabels_->at(index));
            show();
            collapsed_ = false;
            emit collapsedChange(collapsed_, side_);
            return;
        }

        if (stacked_->currentIndex() == index) {
            collapse();
            return;
        }

        stacked_->setCurrentIndex(index);
        stacked_->show();
        title_->setText(itemsLabels_->at(index));
        show();
        collapsed_ = false;
        emit collapsedChange(collapsed_, side_);
    }

    void SideBar::collapse() {
        if (collapsed_) {
            return;
        }

        lastExpandedWidth_ = width();
        stacked_->hide();
        hide();
        collapsed_ = true;
        resize(tabBarWidth_, size().height());
        updateGeometry();
        emit collapsedChange(collapsed_, side_);
    }
}
