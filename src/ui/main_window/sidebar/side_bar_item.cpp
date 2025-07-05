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
