# root
# 기본은 aarch64 크로스 컴파일러를 사용하되, 외부에서 -DCMAKE_C_COMPILER로
# 지정한 경우에는 이를 존중한다.
if(NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
endif()

cmake_minimum_required(VERSION 3.14)
project(mini_server)

file(GLOB SOURCES src/*.c)

add_subdirectory(libs/http)
add_subdirectory(libs/indicators)
add_subdirectory(libs/data_structure)
add_subdirectory(libs/sensors)
add_subdirectory(third_party/cjson)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/cjson
)

## Runtime load libraries via dlopen/dlsym (see src/dynload.c)
## Keep only the minimum link-time dependencies here.
target_link_libraries(${PROJECT_NAME} PRIVATE dl wiringPi pthread)

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