# third_party/cjson

cmake_minimum_required(VERSION 3.14)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

project(cjson)

add_library(${PROJECT_NAME} cJSON.c)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/
)