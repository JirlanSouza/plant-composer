# PlantComposer Architecture

This document outlines the software architecture of the PlantComposer application. The design emphasizes a clean separation of concerns, scalability, and maintainability, following industry-standard patterns.

## 1. High-Level Architecture: MVVM

PlantComposer is built upon a **Model-View-ViewModel (MVVM)** architecture, which is further organized into three distinct layers:

1.  **Domain Layer**: The core of the application. It contains the business logic and data structures (the "Model"). This layer is completely independent and has zero knowledge of the UI, making it highly reusable and testable.
2.  **Adapter Layer**: This layer connects the domain to the outside world. Its primary role is to handle persistence (saving/loading via `IProjectLoader`) and to interface with external systems or libraries.
3.  **UI Layer**: This is the user-facing part of the application (the "View" and "ViewModel"). It is responsible for all presentation and user interaction logic. It depends on the Domain layer but is decoupled from it via the ViewModel.

## 2. Architecture Diagram

[View the architecture diagram](./ARCHITECTURE_DIAGRAM.md)

## 3. Core Domain Models

The domain layer contains the data structures that represent the application's core concepts.

-   **`Project`**: Represents the user's entire workspace. It uses a generic, type-safe, hierarchical tree structure to manage different categories of assets. The design is inspired by the SIMIT project model, using `ProjectCategoryType` to allow for future expansion with items like "Scripts" or "Drivers" alongside the current "Diagrams".
-   **`ProjectNode`**: A polymorphic base class for all items in the project tree, with `FileNode` and `NodeContainer` as concrete implementations. It encapsulates shared logic and business rules like `canBeMoved()` or `canBeRenamed()`.
-   **`Library`**: Represents a collection of component types (`ComponentType`) that can be used in diagrams. This includes the definition of their ports, signals, and constants.
-   **`Diagram`**: Represents a single diagram document. It contains the list of `ComponentInstance`s and the `Connection`s between them.

## 4. The UI Layer: A Modular, MVVM-Based System

The UI is broken down into several key modules, each responsible for a distinct feature of the application. All modules follow the same MVVM principles.

### 4.1. `ui/project` - The Project Explorer

This module is responsible for displaying and managing the project's hierarchical structure.

-   **`ProjectViewManager` (Controller/Mediator)**: This is the main entry point to the feature. It encapsulates the entire project tree, creates its child components, and handles the logic for user interactions like double-clicks and context menus. It translates these low-level UI events into high-level commands for the ViewModel.
-   **`ProjectViewModel` (ViewModel)**: The bridge between the UI and the `Project` domain object. It exposes a clean, testable API for all project-related business logic (e.g., `addNewProjectNode`, `renameProjectNode`). It also manages the state of the clipboard for copy/cut/paste operations. It emits signals when the domain model changes.
-   **`ProjectTreeModel` & `ProjectTreeView` (Model/View)**: A standard Qt Model/View implementation that displays the project hierarchy. The `ProjectTreeModel` listens for signals from the `ProjectViewModel` to perform efficient, granular updates to the tree.

### 4.2. `ui/components_library` - The Components Library

This module is responsible for displaying the available components that can be used in diagrams.

-   **`LibrariesViewManager` (Controller/Mediator)**: Similar to the `ProjectViewManager`, this class manages the components library feature. It loads the libraries (via a `ComponentsLibraryLoader`) and sets up the view and model.
-   **`ComponentsLibraryModel` & `ComponentsLibraryView` (Model/View)**: A tree view that displays the available libraries and their components. The model is responsible for handling the MIME data necessary to initiate a drag-and-drop operation for a component.

### 4.3. `ui/diagram_editor` - The Diagram Editor

This module manages the main workspace where users create and edit diagrams.

-   **`DiagramEditorManager` (Controller/Mediator)**: Manages the tabbed editor interface (`QTabWidget`). It is responsible for opening, closing, and switching between diagram editors.
-   **`DiagramManager` (Session Manager)**: Manages the lifecycle of **active, open** diagrams. It holds a map of `DiagramViewModel`s for the diagrams currently being edited. It is responsible for loading the full diagram data (via the `IProjectLoader`) when a diagram is opened.
-   **`DiagramViewModel` (ViewModel)**: The ViewModel for a single, open diagram. It holds the full `Diagram` domain object and exposes all the logic for editing it (e.g., `addComponent`, `createConnection`).
-   **`DiagramEditorView` & `DiagramEditorScene` (View)**: The `QGraphicsView` and `QGraphicsScene` that provide the canvas for rendering the diagram and handling user interactions like panning, zooming, and dropping components.

## 5. Key Feature Interactions

The power of this architecture lies in how these decoupled modules interact through signals and slots.

-   **Opening a File Node (e.g., a Diagram)**:
    1.  The user double-clicks a file in the `ProjectTreeView`.
    2.  The `ProjectViewManager` translates this into a request to the `ProjectViewModel` via the `openFileNodeRequested` slot, passing a `ProjectContext`.
    3.  The `ProjectViewModel` emits an `openFileNode` signal containing the `FileNode` metadata.
    4.  The `DiagramManager` has a slot connected to this signal. It receives the metadata, loads the full diagram data using the `IProjectLoader`, creates a `DiagramViewModel`, and emits a `diagramOpened` signal.
    5.  The `DiagramEditorManager` has a slot connected to `diagramOpened`. It receives the new `DiagramViewModel` and creates a new editor tab with a `DiagramEditorView` to display it.

-   **Pasting a Node (Cut Operation)**:
    1.  The user right-clicks a node and selects "Cut".
    2.  The `ProjectViewManager` calls `cutProjectNode()` on the `ProjectViewModel`, passing the context of the selected item.
    3.  The `ProjectViewModel` verifies the action is valid (using `canBeMoved()` on the domain node), stores the `ProjectNode` and `ClipboardMode::CUT` in its internal `clipboard_`, and emits `projectNodeCut()`.
    4.  The `ProjectViewManager` receives this signal and enables the "Paste" `QAction`.
    5.  The user right-clicks a valid destination folder and selects "Paste".
    6.  The `ProjectViewManager` calls `pasteProjectNode()` on the `ProjectViewModel`.
    7.  The `ProjectViewModel` performs the core logic: it calls `releaseChild()` on the original parent `NodeContainer` and `addChild()` on the target `NodeContainer`.
    8.  Finally, the `ProjectViewModel` emits a `projectNodePastedAsCut` signal. The `ProjectTreeModel` listens for this signal and updates the `QTreeView` by moving the corresponding `QStandardItem` to its new parent, avoiding a full model rebuild.

-   **Adding a Component to a Diagram**:
    1.  The user drags a component from the `ComponentsLibraryView`.
    2.  The user drops the component onto a `DiagramEditorView`.
    3.  The `DiagramEditorView` emits an `addComponentToScene` signal, containing the component's ID and the drop position.
    4.  The `DiagramEditorManager` has connected this signal to the `addComponent` slot on the corresponding `DiagramViewModel`.
    5.  The `DiagramViewModel` executes the logic, updates the `Diagram` domain object, and emits a `componentAdded` signal.
    6.  The `DiagramEditorScene` has a slot connected to this signal and adds the visual representation of the component to the canvas.