cmake_minimum_required(VERSION 3.14)
project(unit_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    main.cpp
    test_connection.cpp
    test_diagram.cpp
    test_components_library.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        GTest::gtest_main
        gmock
        Qt6::Core
        domain
        adapters
        ui
)

add_test(NAME unit_tests COMMAND unit_tests)
