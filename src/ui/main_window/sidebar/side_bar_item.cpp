/*
 * plant-composer
 * Copyright (C) 2025 Jirlan Souza
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

#include "side_bar_item.h"

#include <QToolButton>
#include <QPainter>
#include <QStyleOptionToolButton>

namespace ui::layout {
    SideBarItem::SideBarItem(
        const int index,
        const QString &title,
        QWidget *parent
    ): QToolButton(parent), index_(index) {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        setText(title);
        setAutoRaise(true);
        setFocusPolicy(Qt::NoFocus);
        setStyleSheet(
            R"(
        QToolButton {
            background: transparent;
            border: none;
            padding: 4px;
        }
        QToolButton:hover {
            background: rgba(255, 255, 255, 0.05);
        }
        QToolButton:checked {
            background: rgba(100, 100, 255, 0.2);
        }
    )"
        );

        connect(this, &QToolButton::clicked, this, &SideBarItem::onClicked);
    }

    int SideBarItem::getIndex() const {
        return index_;
    }


    void SideBarItem::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QStyleOptionToolButton option;
        initStyleOption(&option);
        style()->drawPrimitive(QStyle::PE_PanelButtonTool, &option, &painter, this);

        painter.save();
        painter.translate(width(), 0);
        painter.rotate(90);

        QRect rotatedRect(0, 0, height(), width());
        style()->drawItemText(&painter, rotatedRect, Qt::AlignCenter, palette(), true, text());
        painter.restore();
    }

    QSize SideBarItem::sizeHint() const {
        QSize textSize = fontMetrics().size(0, text());
        return QSize(32, textSize.width() + 56);
    }

    void SideBarItem::onClicked() {
        emit clicked(index_);
    }
};
