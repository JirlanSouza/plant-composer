# PlantComposer

PlantComposer is a simulation platform designed for the virtual commissioning and testing of automation systems. It provides a modular and extensible environment for modeling industrial processes and their control logic.

## Features

*   **Intuitive User Interface:** A graphical interface with a main application window and a diagram editor for creating and visualizing models.
*   **Extensible Component Libraries:** Load and manage component definitions from JSON files, allowing for easy expansion of available components.
*   **Customizable Application Settings:** Configure various application settings to tailor the environment to your needs.
*   **Multi-language Support:** The application supports internationalization, allowing for use in multiple languages.

## Architecture

PlantComposer is built with a layered architecture, emphasizing separation of concerns:

*   **Domain Layer:** Contains the pure C++20 core logic, independent of UI or external frameworks.
*   **Adapter/Infrastructure Layer:** Provides concrete implementations for external integrations (UI, couplings, persistence).
*   **Application Layer:** Handles application initialization and composition.

## Installation

*(To be added: Instructions for building and installing the application.)*

## Usage

*(To be added: Instructions and examples for using PlantComposer to create and run simulations.)*

## License

See the [LICENSE](LICENSE) file for details.