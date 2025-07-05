#pragma once
#include <QToolButton>

namespace ui::layout {
    class SideBarItem : public QToolButton {
        Q_OBJECT

    public:
        SideBarItem(int index, const QString &title, QWidget *parent);

        QSize sizeHint() const override;

        int getIndex() const;

    signals:
        void clicked(int index);

    protected:
        void paintEvent(QPaintEvent *) override;

    private:
        int index_;

    private slots:
        void onClicked();
    };
}
