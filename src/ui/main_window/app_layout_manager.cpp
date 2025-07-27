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

#include "app_layout_manager.h"

#include <qboxlayout.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

namespace app_layout {
    AppLayoutManager::AppLayoutManager(QMainWindow *mainWindow, app_actions::ActionsManager *actionsManager): QObject(
            mainWindow
        ),
        mainWindow_(mainWindow),
        actionsManager_(actionsManager) {
        menuBar_ = new QMenuBar(mainWindow_);
        mainToolBar_ = new QToolBar(mainWindow_);
        statusBar_ = new QStatusBar(mainWindow_);
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

    void AppLayoutManager::setupManuBar() const {
        mainWindow_->setMenuBar(menuBar_);

        const auto menuFile = menuBar_->addMenu(tr("File"));
        menuFile->addActions(actionsManager_->getActions(app_actions::ActionGroupType::File));

        const auto menuEdit = menuBar_->addMenu(tr("Edit"));
        menuEdit->addActions(actionsManager_->getActions(app_actions::ActionGroupType::Edit));

        const auto menuView = menuBar_->addMenu(tr("View"));
        menuView->addActions(actionsManager_->getActions(app_actions::ActionGroupType::View));

        const auto menuSimulation = menuBar_->addMenu(tr("Simulation"));
        menuSimulation->addActions(actionsManager_->getActions(app_actions::ActionGroupType::Simulation));

        const auto menuOptions = menuBar_->addMenu(tr("Options"));
        menuOptions->addActions(actionsManager_->getActions(app_actions::ActionGroupType::Options));

        const auto menuWindow = menuBar_->addMenu(tr("Window"));
        menuWindow->addActions(actionsManager_->getActions(app_actions::ActionGroupType::Window));
    }

    void AppLayoutManager::setupMainToolBar() const {
        mainWindow_->addToolBar(mainToolBar_);
        mainToolBar_->setIconSize(QSize(22, 22));
        mainToolBar_->setFloatable(false);
        mainToolBar_->setMovable(false);
        mainToolBar_->setToolButtonStyle(Qt::ToolButtonIconOnly);
        mainToolBar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        mainToolBar_->setMaximumHeight(32);

        mainToolBar_->addActions(actionsManager_->getActions(app_actions::ActionGroupType::ToolbarFile));
        mainToolBar_->addSeparator();
        mainToolBar_->addActions(actionsManager_->getActions(app_actions::ActionGroupType::ToolbarEdit));
    }

    void AppLayoutManager::setupStatusBar() const {
        statusBar_->setSizeGripEnabled(false);
        mainWindow_->setStatusBar(statusBar_);
    }
}
