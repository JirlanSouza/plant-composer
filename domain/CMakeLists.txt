file(GLOB_RECURSE DOMAIN_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(domain STATIC ${DOMAIN_SOURCES})

target_include_directories(domain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})