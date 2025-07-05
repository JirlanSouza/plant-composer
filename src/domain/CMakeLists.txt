add_library(domain STATIC)
target_sources(domain
        PUBLIC
        shared/id_factory.h

        components_library/model/component.h
        components_library/model/component_io.h
        components_library/model/data_type.h
        components_library/model/data_type.cpp
        components_library/model/library.h
        components_library/components_library_loader.h

        diagram/model/component_instance.h
        diagram/model/component_instance.cpp
        diagram/model/node_transform.h
        diagram/model/diagram.h
        diagram/model/diagram.cpp
        diagram/component_instance_factory.h
        diagram/component_instance_factory.cpp

        settings/app_settings.h
)

target_include_directories(domain PUBLIC ../)