file(GLOB_RECURSE ADAPTERS_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(adapters STATIC ${ADAPTERS_SOURCES})

target_include_directories(adapters PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(adapters
        PRIVATE Qt6::Core
        nlohmann_json::nlohmann_json
        domain
)