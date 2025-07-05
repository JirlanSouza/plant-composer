#pragma once
#include <qtabwidget.h>

#include "domain/settings/app_settings.h"
#include "diagram_service.h"
#include "editor_widget/diagram_editor_scene.h"
#include "editor_widget/diagram_editor_view.h"
#include "editor_widget/diagram_view_model.h"

namespace dcl = domain::components_library;
namespace dst = domain::settings;

namespace ui::diagram_editor {
    struct DiagramEditorTab {
        DiagramViewModel *diagram;
        DiagramEditorView *diagramEditorView_;
        DiagramEditorScene *diagramEditorScene_;
    };

    class DiagramEditorManager : public QObject {
        Q_OBJECT

    public:
        explicit DiagramEditorManager(DiagramService *diagramService, QWidget *parent = nullptr);

        ~DiagramEditorManager() override;

        [[nodiscard]] QWidget *getView() const;

    private:
        DiagramService *diagramService_;
        QTabWidget *editorArea_;
        std::unordered_map<std::string, DiagramEditorTab> diagramEditorTabs_;
        std::vector<std::string> editorTabsOrder_;

    private slots:
        void onOpenedDiagram(const std::string &diagramId);

        void onTabMoved(int from, int to);

        void onTabCloseRequested(int index);
    };
}
