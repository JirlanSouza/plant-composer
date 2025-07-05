```mermaid
graph TD
    subgraph "Application"
        A[main.cpp] --> B(plant_composer_application)
    end

    subgraph "UI (Qt)"
        B --> C{main_window}
        C --> D{app_layout_manager}
        C --> E{libraries_view_manager}
        C --> F{chart_editor_manager}
        E --> G{components_library_view}
        E --> H{components_library_model}
        F --> I{chart_editor_view}
        F --> J{chart_editor_scene}
        F --> K{chart_model}
        I --> L{component_instance_view}
    end

    subgraph "Adapters"
        M(qt_app_settings) --> N[AppSettings]
        O(json_components_library_loader) --> P[ComponentsLibraryLoader]
    end

    subgraph "Domain"
        P --> Q{library.h}
        N --> R{app_settings.h}
        S{chart.h} --> T{component_instance.h}
        T --> U{component.h}
        U --> V{component_io.h}
        V --> W{data_type.h}
    end

    subgraph "Common Utils"
        X(id_utils)
        Y(json_utils)
    end

    classDef domain fill: #f9f, stroke: #333, stroke-width: 2px, color: #000;
    classDef adapter fill: #9cf, stroke: #333, stroke-width: 2px, color: #000;
    classDef ui fill: #9c9, stroke: #333, stroke-width: 2px, color: #000;
    classDef app fill: #c99, stroke: #333, stroke-width: 2px, color: #000;
    classDef utils fill: #ccc, stroke: #333, stroke-width: 2px, color: #000;
    class A, B app;
    class C, D, E, F, G, H, I, J, K, L ui;
    class M, O adapter;
    class N, P, Q, R, S, T, U, V, W domain;
    class X, Y utils;
```
