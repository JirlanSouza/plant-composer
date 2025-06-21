file(GLOB_RECURSE COMMON_UTILS_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(common_utils STATIC ${COMMON_UTILS_SOURCES})

target_include_directories(common_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(common_utils
        nlohmann_json::nlohmann_json
)