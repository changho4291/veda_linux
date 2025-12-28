# third_party/cjson

cmake_minimum_required(VERSION 3.14)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

project(cjson)

add_library(${PROJECT_NAME} cJSON.c)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:install>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION third_party
        LIBRARY DESTINATION third_party
        RUNTIME DESTINATION bin)

install(FILES ${PROJECT_SOURCE_DIR}/cJSON.h
        DESTINATION third_party)