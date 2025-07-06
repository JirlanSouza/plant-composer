cmake_minimum_required(VERSION 3.14)
project(integration_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(integration_tests
        test_json_components_library_loader.cpp
        test_qt_app_settings.cpp
        test_flat_buffer_project_loader.cpp
)

target_link_libraries(integration_tests
        PRIVATE
        GTest::gtest_main
        Qt6::Core
        domain
        adapters
)

add_test(NAME integration_tests COMMAND integration_tests)
