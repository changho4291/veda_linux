# root

cmake_minimum_required(VERSION 3.14)
project(veda_mini_project)

file(GLOB SOURCES *.c)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(http)
add_subdirectory(indicators)

target_link_libraries(${PROJECT_NAME} PRIVATE http indicators)

# install(TARGETS main
#         RUNTIME DESTINATION bin)

# install(EXPORT calcTargets
#         FILE calcTargets.cmake
#         DESTINATION lib/cmake/calc)