#pragma once
#include <QMainWindow>

#include "sidebar/side_bar.h"
#include "sidebar/side_bar_tabs.h"

namespace ui::layout {
    class AppLayoutManager : public QObject {
        Q_OBJECT

    public:
        AppLayoutManager(QMainWindow *mainWindow);

        ~AppLayoutManager() override;

        void addSideBarView(const QString &label, QWidget *view, Side side) const;

        void setCentralWidget(QWidget *widget) const;

    private:
        QMainWindow *mainWindow_{};
        QMenuBar *menuBar_;
        QToolBar *mainToolBar_;
        QStatusBar *statusBar_;
        SideBarTabs *leftTabBar_;
        SideBarTabs *rightTabBar_;
        SideBar *leftSideBar_;
        SideBar *rightSideBar_;

        QAction *actionNewPlant_{};
        QAction *actionOpenPlant_{};
        QAction *actionSavePlant_{};
        QAction *actionSaveAs_{};
        QAction *actionUndo_{};
        QAction *actionRedo_{};
        QAction *actionShowLibrary_{};
        QAction *actionStart_{};
        QAction *actionStop_{};
        QAction *actionSettings_{};
        QAction *actionArrange_{};

        void setupManuBar();

        void setupMainToolBar();

        void setupStatusBar() const;
    };
}
