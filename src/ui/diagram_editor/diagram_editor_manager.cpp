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

#include "diagram_editor_manager.h"

#include <QTabBar>

namespace ui::diagram_editor {
    DiagramEditorManager::DiagramEditorManager(DiagramManager *diagramService, QWidget *parent): QObject(parent),
        diagramService_(diagramService) {
        editorArea_ = new QTabWidget();
        editorArea_->setDocumentMode(true);
        editorArea_->setMovable(true);
        editorArea_->setTabsClosable(true);

        connect(editorArea_, &QTabWidget::tabCloseRequested, this, &DiagramEditorManager::onTabCloseRequested);
        connect(editorArea_->tabBar(), &QTabBar::tabMoved, this, &DiagramEditorManager::onTabMoved);

        diagramService_->openDiagram("lkfdg0q9jgdjnvofruifdek");
        onOpenedDiagram("lkfdg0q9jgdjnvofruifdek");

        diagramService_->openDiagram("lkfdg0q9jgdjnvofruif438dsf");
        onOpenedDiagram("lkfdg0q9jgdjnvofruif438dsf");

        diagramService_->openDiagram("lkfdg0q9jgdjnvofruisdu82k");
        onOpenedDiagram("lkfdg0q9jgdjnvofruisdu82k");
    }

    DiagramEditorManager::~DiagramEditorManager() = default;

    void DiagramEditorManager::onOpenedDiagram(const std::string &diagramId) {
        if (diagramEditorTabs_.contains(diagramId)) {
            editorArea_->setCurrentWidget(diagramEditorTabs_[diagramId].diagramEditorView_);
            return;
        }

        DiagramViewModel *diagram = diagramService_->getDiagram(diagramId);
        auto *diagramEditorView = new DiagramEditorView(diagramId, editorArea_);
        auto *diagramEditorScene = new DiagramEditorScene(diagram, diagramEditorView);
        diagramEditorScene->setSceneRect(0, 0, diagram->getWidth(), diagram->getHeight());
        diagramEditorView->setScene(diagramEditorScene);

        const DiagramEditorTab diagramEditorTab{diagram, diagramEditorView, diagramEditorScene};
        diagramEditorTabs_[diagramId] = diagramEditorTab;
        editorArea_->addTab(diagramEditorTab.diagramEditorView_, QString::fromStdString(diagram->getName()));
        editorTabsOrder_.push_back(diagramId);

        connect(
            diagramEditorView,
            &DiagramEditorView::addComponentToScene,
            this,
            [diagram](const std::string &diagramId, int libraryId, int componentId, QPointF posi) {
                diagram->addComponent(libraryId, componentId, posi);
            }
        );
    }

    void DiagramEditorManager::onTabMoved(const int from, const int to) {
        const auto diagramId = editorTabsOrder_[from];
        editorTabsOrder_.erase(editorTabsOrder_.begin() + from);
        editorTabsOrder_.insert(editorTabsOrder_.begin() + to, diagramId);
    }

    void DiagramEditorManager::onTabCloseRequested(const int index) {
        diagramEditorTabs_.erase(editorTabsOrder_[index]);
        editorArea_->removeTab(index);
        editorTabsOrder_.erase(editorTabsOrder_.begin() + index);
    }

    QWidget *DiagramEditorManager::getView() const {
        return editorArea_;
    }
}
