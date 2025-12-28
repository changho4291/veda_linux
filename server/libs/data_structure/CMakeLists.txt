# /data_structure

cmake_minimum_required(VERSION 3.14)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

project(data_structure)

file(GLOB SOURCES *.c)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:install>
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION libs
        LIBRARY DESTINATION libs
        RUNTIME DESTINATION bin)