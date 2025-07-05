#include "side_bar_tabs.h"

#include <QVBoxLayout>

#include "side_bar_item.h"

namespace ui::layout {
    SideBarTabs::SideBarTabs(const int width, QWidget *parent): QToolBar(parent), layout_(new QVBoxLayout(this)) {
        setFixedWidth(width);
        layout_->setContentsMargins(0, 0, 0, 0);
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignTop);

        setMovable(false);
        setFloatable(false);
        setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
        setOrientation(Qt::Vertical);
        setStyleSheet("QToolBar { border: none; background: transparent; }");
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
