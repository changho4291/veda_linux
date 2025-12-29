# root
cmake_minimum_required(VERSION 3.14)
project(mini_client)

file(GLOB SOURCES src/*.c)

add_subdirectory(third_party/cjson)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:install>
)

target_link_libraries(${PROJECT_NAME} PRIVATE cjson)