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
