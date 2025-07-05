add_library(common_utils STATIC)

target_sources(common_utils
        PUBLIC
        id/id_utils.h
        id/id_utils.cpp

        json/json_utils.h
        json/json_utils.cpp
)

target_include_directories(common_utils PUBLIC .)

target_link_libraries(common_utils
        nlohmann_json::nlohmann_json
        Qt6::Core
)