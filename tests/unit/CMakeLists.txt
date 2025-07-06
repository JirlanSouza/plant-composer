cmake_minimum_required(VERSION 3.14)
project(unit_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Test REQUIRED)

add_executable(unit_tests
    main.cpp
    test_connection.cpp
    test_project.cpp
    test_connection.h
    test_project.h
    test_diagram.h
    test_diagram.cpp
    test_components_library.h
    test_components_library.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        Qt6::Test
        domain
        adapters
)

add_test(NAME unit_tests COMMAND unit_tests)
