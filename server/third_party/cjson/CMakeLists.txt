# third_party/cjson

cmake_minimum_required(VERSION 3.14)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

project(cjson)

add_library(${PROJECT_NAME} cJSON.c)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:install>
)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread wiringPi)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION third_party
        LIBRARY DESTINATION third_party
        RUNTIME DESTINATION bin)

install(FILES ${PROJECT_SOURCE_DIR}/cJSON.h
        DESTINATION third_party)