#pragma once
#include <QToolBar>

namespace ui::layout {
    class SideBarTabs : public QToolBar {
        Q_OBJECT

    public:
        SideBarTabs(int width = 32, QWidget *parent = nullptr);

        ~SideBarTabs() override;

        int addTab(const QString &label);

        int tabsCount() const;

        int currentTabIndex() const;

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
