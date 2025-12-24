# root

cmake_minimum_required(VERSION 3.14)
project(veda_mini_project)

file(GLOB SOURCES *.c)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME} PRIVATE http)

# install(TARGETS main
#         RUNTIME DESTINATION bin)

# install(EXPORT calcTargets
#         FILE calcTargets.cmake
#         DESTINATION lib/cmake/calc)