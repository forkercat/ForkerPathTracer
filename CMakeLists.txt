cmake_minimum_required(VERSION 3.17)
project(ForkerPathTracer)

set(CMAKE_CXX_STANDARD 14)

find_package(spdlog REQUIRED)

#########################################################

set(COMMON_SOURCE src/common/common.cpp src/common/color.cpp)

set(COMMON_HEADERS
    src/common/common.h
    src/common/constant.h
    src/common/utility.h
    src/common/check.h
    src/common/geometry.h
    src/common/stringprint.h
    src/common/color.h)

add_library(common STATIC
    ${COMMON_SOURCE}
    ${COMMON_HEADERS})

set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(common PRIVATE spdlog::spdlog)

#########################################################

set(CORE_SOURCE
    src/core/core.cpp
    src/core/scene.cpp
    src/core/bvh.cpp
    src/core/triangle.cpp
    src/core/loader.cpp)

set(CORE_HEADERS
    src/core/core.h
    src/core/camera.h
    src/core/hittable.h
    src/core/sphere.h
    src/core/material.h
    src/core/ray.h
    src/core/bounds.h
    src/core/scene.h
    src/core/bvh.h
    src/core/triangle.h
    src/core/loader.h)

add_library(core STATIC
    ${CORE_SOURCE}
    ${CORE_HEADERS})

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(core PRIVATE spdlog::spdlog)

set(ALL_LIBS
    spdlog::spdlog
    common
    core)

INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(src/common)
INCLUDE_DIRECTORIES(src/core)

#########################################################

add_executable(ForkerPathTracer src/main/main.cpp)
target_link_libraries(ForkerPathTracer PRIVATE ${ALL_LIBS})

#########################################################

add_executable(Test src/test/test.cpp)
target_link_libraries(Test PRIVATE ${ALL_LIBS})

# Create output directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)
#file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/output)