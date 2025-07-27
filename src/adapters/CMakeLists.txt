add_library(adapters STATIC)

target_sources(adapters
        PUBLIC
        spdlog_logger.h
        spdlog_logger.cpp
        quuid_id_factory.h
        quuid_id_factory.cpp

        components_library/json/components_library_json.cpp
        components_library/json/components_library_json.h
        components_library/json_components_library_loader.cpp
        components_library/json_components_library_loader.h

        settings/qt_app_settings.cpp
        settings/qt_app_settings.h

        project/flat_buffer_project_loader.h
        project/flat_buffer_project_loader.cpp
        project/flat_buffer_project_parser.h
        project/flat_buffer_project_parser.cpp
)

target_include_directories(adapters PUBLIC ../
)

target_link_libraries(adapters
        PRIVATE Qt6::Core
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        domain
)