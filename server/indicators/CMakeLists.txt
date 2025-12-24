# /indicators

cmake_minimum_required(VERSION 3.14)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

project(indicators)

file(GLOB SOURCES *.c)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:install>
)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread wiringPi)

# install(TARGETS calc
#         EXPORT calcTargets
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib
#         RUNTIME DESTINATION bin)

# install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
#         DESTINATION include)