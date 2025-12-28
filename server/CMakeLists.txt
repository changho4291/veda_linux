# root
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)

cmake_minimum_required(VERSION 3.14)
project(mini_server)

file(GLOB SOURCES src/*.c)

add_subdirectory(libs/http)
add_subdirectory(libs/indicators)
add_subdirectory(libs/data_structure)
add_subdirectory(libs/sensors)
add_subdirectory(third_party/cjson)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE http indicators data_structure sensors cjson dl wiringPi)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)

install(EXPORT httpTargets
        FILE httpTargets.cmake
        DESTINATION libs/cmake/http)

install(EXPORT indicatorsTargets
        FILE indicatorsTargets.cmake
        DESTINATION libs/cmake/indicators)

install(EXPORT data_structureTargets
        FILE data_structureTargets.cmake
        DESTINATION libs/cmake/data_structure)

install(EXPORT sensorsTargets
        FILE sensorsTargets.cmake
        DESTINATION libs/cmake/sensors)

install(EXPORT cjsonTargets
        FILE cjsonTargets.cmake
        DESTINATION third_party/cmake/cjson)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include)