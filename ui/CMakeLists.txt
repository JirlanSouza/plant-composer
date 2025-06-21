file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS *.cpp *.h *.ui)

add_library(ui STATIC ${UI_SOURCES})

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ui
        PRIVATE Qt6::Widgets
)
