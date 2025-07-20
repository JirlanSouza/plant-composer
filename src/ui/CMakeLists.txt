add_library(ui STATIC)

target_sources(ui
        PUBLIC
        mime_types.h
        main_window/main_window.h
        main_window/main_window.cpp
        main_window/app_layout_manager.h
        main_window/app_layout_manager.cpp

        actions_manager/actions_manager.h
        actions_manager/actions_manager.cpp
        main_window/sidebar/side_bar.h
        main_window/sidebar/side_bar.cpp
        main_window/sidebar/side_bar_tabs.h
        main_window/sidebar/side_bar_tabs.cpp
        main_window/sidebar/side_bar_item.h
        main_window/sidebar/side_bar_item.cpp

        project/project_view_manager.h
        project/project_view_manager.cpp
        project/project_view_model.h
        project/project_view_model.cpp
        project/project_tree/project_tree_item_type.h
        project/project_tree/project_tree_item_type.cpp
        project/project_tree/project_tree_model.h
        project/project_tree/project_tree_model.cpp
        project/project_tree/project_tree_view.h
        project/project_tree/project_tree_view.cpp
        project/new_project_dialog.h
        project/new_project_dialog.cpp

        components_library/libraries_view_manager.h
        components_library/libraries_view_manager.cpp
        components_library/component_tree/components_library_model.h
        components_library/component_tree/components_library_model.cpp
        components_library/component_tree/components_library_view.h
        components_library/component_tree/components_library_view.cpp
        components_library/properties_dialog/component_properties_view.h
        components_library/properties_dialog/component_properties_view.cpp

        diagram_editor/diagram_editor_manager.cpp
        diagram_editor/diagram_editor_manager.h
        diagram_editor/diagram_manager.h
        diagram_editor/diagram_manager.cpp
        diagram_editor/editor_widget/component_view.cpp
        diagram_editor/editor_widget/component_view.h
        diagram_editor/editor_widget/component_view_model.cpp
        diagram_editor/editor_widget/component_view_model.h
        diagram_editor/editor_widget/diagram_editor_scene.cpp
        diagram_editor/editor_widget/diagram_editor_scene.h
        diagram_editor/editor_widget/diagram_editor_view.cpp
        diagram_editor/editor_widget/diagram_editor_view.h
        diagram_editor/editor_widget/diagram_view_model.cpp
        diagram_editor/editor_widget/diagram_view_model.h
)

target_include_directories(ui PUBLIC .)

target_link_libraries(ui
        PRIVATE Qt6::Widgets
        Qt6::SvgWidgets
        domain
        adapters
        common_utils
)

