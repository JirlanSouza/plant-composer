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

#include "app_layout_manager.h"

#include <qboxlayout.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

namespace ui::layout {
    AppLayoutManager::AppLayoutManager(QMainWindow *mainWindow): QObject(mainWindow), mainWindow_(mainWindow) {
        menuBar_ = new QMenuBar(mainWindow_);
        mainToolBar_ = new QToolBar(mainWindow_);
        statusBar_ = new QStatusBar(mainWindow_);
        setupManuBar();
        setupMainToolBar();
        setupStatusBar();

        leftTabBar_ = new SideBarTabs(32, mainWindow_);
        leftSideBar_ = new SideBar(mainWindow_, LEFT);
        mainWindow_->addToolBar(Qt::LeftToolBarArea, leftTabBar_);
        mainWindow_->addDockWidget(Qt::LeftDockWidgetArea, leftSideBar_);
        connect(leftTabBar_, &SideBarTabs::indexChanged, leftSideBar_, &SideBar::onItemChanged);

        rightTabBar_ = new SideBarTabs(32, mainWindow_);
        rightSideBar_ = new SideBar(mainWindow_, RIGHT);
        mainWindow_->addToolBar(Qt::RightToolBarArea, rightTabBar_);
        mainWindow_->addDockWidget(Qt::RightDockWidgetArea, rightSideBar_);
        connect(rightTabBar_, &SideBarTabs::indexChanged, rightSideBar_, &SideBar::onItemChanged);
    }

    AppLayoutManager::~AppLayoutManager() = default;

    void AppLayoutManager::addSideBarView(const QString &label, QWidget *view, const Side side) const {
        if (side == LEFT) {
            leftTabBar_->addTab(label);
            leftSideBar_->addItem(label, view);
            return;
        }

        rightTabBar_->addTab(label);
        rightSideBar_->addItem(label, view);
    }

    void AppLayoutManager::setCentralWidget(QWidget *widget) const {
        mainWindow_->setCentralWidget(widget);
    }

    void AppLayoutManager::setupManuBar() {
        mainWindow_->setMenuBar(menuBar_);

        const auto menuFile = menuBar_->addMenu(tr("File"));
        actionNewPlant_ = new QAction(QIcon::fromTheme("document-new"), tr("New plant"), this);
        actionOpenPlant_ = new QAction(QIcon::fromTheme("document-open"), tr("Open plant"), this);
        actionSavePlant_ = new QAction(QIcon::fromTheme("document-save"), tr("Save"), this);
        actionSaveAs_ = new QAction(QIcon::fromTheme("document-save-as"), tr("Save as"), this);
        menuFile->addActions({actionNewPlant_, actionOpenPlant_, actionSavePlant_, actionSaveAs_});

        // Edit Menu
        const auto menuEdit = menuBar_->addMenu(tr("Edit"));
        actionUndo_ = new QAction(QIcon::fromTheme("edit-undo"), tr("Undo"), this);
        actionRedo_ = new QAction(QIcon::fromTheme("edit-redo"), tr("Redo"), this);
        menuEdit->addActions({actionUndo_, actionRedo_});

        // View Menu
        const auto menuView = menuBar_->addMenu(tr("View"));
        actionShowLibrary_ = new QAction(tr("Show library"), this);
        menuView->addAction(actionShowLibrary_);

        // Simulation Menu
        const auto menuSimulation = menuBar_->addMenu(tr("Simulation"));
        actionStart_ = new QAction(tr("Start"), this);
        actionStop_ = new QAction(tr("Stop"), this);
        menuSimulation->addActions({actionStart_, actionStop_});

        // Options Menu
        const auto menuOptions = menuBar_->addMenu(tr("Options"));
        actionSettings_ = new QAction(tr("Settings"), this);
        menuOptions->addAction(actionSettings_);

        // Window Menu
        const auto menuWindow = menuBar_->addMenu(tr("Window"));
        actionArrange_ = new QAction(tr("Arange"), this);
        menuWindow->addAction(actionArrange_);
    }

    void AppLayoutManager::setupMainToolBar() {
        mainWindow_->addToolBar(mainToolBar_);
        mainToolBar_->setIconSize(QSize(22, 22));
        mainToolBar_->setFloatable(false);
        mainToolBar_->setToolButtonStyle(Qt::ToolButtonIconOnly);
        mainToolBar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        mainToolBar_->setMaximumHeight(32);

        mainToolBar_->addActions(
            {
                actionNewPlant_,
                actionOpenPlant_,
                actionSavePlant_,
                actionSaveAs_
            }
        );
        mainToolBar_->addSeparator();
        mainToolBar_->addActions(
            {
                actionUndo_,
                actionRedo_
            }
        );
    }

    void AppLayoutManager::setupStatusBar() const {
        statusBar_->setSizeGripEnabled(false);
        mainWindow_->setStatusBar(statusBar_);
    }
}
