cmake_minimum_required(VERSION 3.14)
project(integration_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.9.1/macos/lib/cmake/Qt6")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools SvgWidgets)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(integration_tests
    test_json_components_library_loader.cpp
    test_qt_app_settings.cpp
    test_component_view_model.cpp
    test_diagram_view_model.cpp
)

target_link_libraries(integration_tests
    PRIVATE
        GTest::gtest_main
        gmock
        Qt6::Widgets
        Qt6::SvgWidgets
        domain
        adapters
        ui
        common_utils
)

add_test(NAME integration_tests COMMAND integration_tests)
