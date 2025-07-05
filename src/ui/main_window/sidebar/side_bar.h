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
