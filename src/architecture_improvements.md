# Architecture and Naming Improvements

This document outlines suggested improvements to the PlantComposer architecture and naming conventions to enhance
clarity, scalability, and maintainability, inspired by the SIMIT manual.

## Architectural Improvements

### Suggestion 1: Introduce a dedicated `simulation` Layer

**Rationale**: To separate the static *design* of a system from its dynamic *execution*, which is the core purpose of a
simulation tool.

- [ ] Create a new top-level directory: `src/simulation`.
- [ ] Implement a `SimulationEngine` class within the `simulation` layer to manage the simulation loop (time steps,
  start, stop) and data flow.
- [ ] Define an `ISimulatable` interface (or abstract class) that components must implement to define their simulation
  behavior (e.g., `initialize()`, `onTick()`).
- [ ] Create a `SimulationState` class to hold the live state of all signals and variables during a simulation.

### Suggestion 2: Purify the ViewModel Layer

**Rationale**: To increase decoupling between the View and the Model, making the View simpler and more reusable,
adhering more strictly to MVVM principles.

- [ ] Create a dedicated `ComponentViewModel` class that wraps a domain component and exposes Qt-friendly properties (
  `Q_PROPERTY`) for the View to bind to. This prevents the View from depending directly on the domain model.
- [ ] Refactor by merging `ChartInterface` (commands) and `ChartModel` (data wrapper) into a single, comprehensive
  `DiagramViewModel` class to unify the ViewModel's responsibilities.

## Naming and Clarity Improvements

**Rationale**: To use names that more accurately reflect the application's purpose (simulation and diagramming), making
the code more intuitive.

| Current Name (File/Class)    | Suggested Name                        | Implemented |
|:-----------------------------|:--------------------------------------|:------------|
| `domain/chart_editor` (dir)  | `domain/diagram`                      | `- [ ]`     |
| `Chart`                      | `Diagram`                             | `- [ ]`     |
| `ComponentInstance`          | `DiagramComponent` or `Node`          | `- [ ]`     |
| `GraphicItemPosition`        | `DiagramPosition` or `NodeTransform`  | `- [ ]`     |
| `ui/chart_editor` (dir)      | `ui/diagram_editor`                   | `- [ ]`     |
| `ChartEditorManager`         | `DiagramEditorManager`                | `- [ ]`     |
| `ChartInterface`             | `DiagramService` or `DiagramCommands` | `- [ ]`     |
| `ComponentsLibraryLoader`    | `ComponentLibraryLoader`              | `- [ ]`     |
| `plant_composer_application` | `CompositionRoot` or `AppManager`     | `- [ ]`     |

