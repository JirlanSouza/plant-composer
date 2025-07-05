add_library(domain STATIC)
target_sources(domain
        PUBLIC
        components_library/model/component.h
        components_library/model/component_io.h
        components_library/model/data_type.h
        components_library/model/data_type.cpp
        components_library/model/library.h
        components_library/components_library_loader.h

        diagram/model/component_instance.h
        diagram/model/component_instance.cpp
        diagram/model/graphic_item_position.h
        diagram/model/diagram.h
        diagram/model/diagram.cpp

        settings/app_settings.h
)

target_include_directories(domain PUBLIC ../)