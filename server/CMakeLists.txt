# root

cmake_minimum_required(VERSION 3.14)
project(veda_mini_project)

file(GLOB SOURCES src/*.c)

add_subdirectory(libs/http)
add_subdirectory(libs/indicators)
add_subdirectory(libs/data_structure)
add_subdirectory(libs/sensors)
add_subdirectory(third_party/cjson)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE http indicators data_structure sensors cjson dl)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)

install(EXPORT httpTargets
        FILE httpTargets.cmake
        DESTINATION lib/cmake/http)

install(EXPORT indicatorsTargets
        FILE indicatorsTargets.cmake
        DESTINATION lib/cmake/indicators)

install(EXPORT data_structureTargets
        FILE data_structureTargets.cmake
        DESTINATION lib/cmake/data_structure)

install(EXPORT sensorsTargets
        FILE sensorsTargets.cmake
        DESTINATION lib/cmake/sensors)

install(EXPORT cjsonTargets
        FILE cjsonTargets.cmake
        DESTINATION third_party/cmake/cjson)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include)