# root

cmake_minimum_required(VERSION 3.14)
project(veda_mini_project)

file(GLOB SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(libs/http)
add_subdirectory(libs/indicators)
add_subdirectory(libs/data_structure)

target_link_libraries(${PROJECT_NAME} PRIVATE http indicators data_structure)

# install(TARGETS main
#         RUNTIME DESTINATION bin)

# install(EXPORT calcTargets
#         FILE calcTargets.cmake
#         DESTINATION lib/cmake/calc)