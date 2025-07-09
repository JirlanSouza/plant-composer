cmake_minimum_required(VERSION 3.16)

project(plant-composer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.9.1/macos/lib/cmake/Qt6")
add_compile_definitions(SOURCE_ROOT="${CMAKE_SOURCE_DIR}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools SvgWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools SvgWidgets)
find_package(nlohmann_json 3.12.0 REQUIRED)

set(TS_FILES
        translations/plant-composer_en_US.ts
        translations/plant-composer_pt_BR.ts
)

set(PROJECT_SOURCES
        src/main.cpp
        src/plant_composer_application.cpp
        src/plant_composer_application.h
        ${TS_FILES}
)
add_subdirectory(src/domain)
add_subdirectory(src/adapters)
add_subdirectory(src/ui)
add_subdirectory(src/common_utils)
add_subdirectory(tests/unit)
add_subdirectory(tests/integration)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_resources(RESOURCES_RCC src/resources/resources.qrc)
    qt_add_executable(plant-composer
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${RESOURCES_RCC}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET plant-composer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(plant-composer SHARED
                ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(plant-composer
                ${PROJECT_SOURCES}
        )
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(plant-composer
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::SvgWidgets
        nlohmann_json::nlohmann_json
        domain
        adapters
        ui
        common_utils
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.plant-composer)
endif ()
set_target_properties(plant-composer PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS plant-composer
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(plant-composer)
endif ()
