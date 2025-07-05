#include "side_bar.h"

#include <QMainWindow>
#include <qscrollarea.h>
#include <QVBoxLayout>
#include <QWindow>

namespace ui::layout {
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
