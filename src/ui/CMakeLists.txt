file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS *.cpp *.h *.ui)

add_library(ui STATIC ${UI_SOURCES})

target_include_directories(ui PUBLIC .)

target_link_libraries(ui
        PRIVATE Qt6::Widgets
        Qt6::SvgWidgets
        domain
        adapters
)

