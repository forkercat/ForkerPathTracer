cmake_minimum_required(VERSION 3.17)
project(ForkerPathTracer)

set(CMAKE_CXX_STANDARD 14)

find_package(spdlog REQUIRED)

#########################################################
# Common
set(COMMON_SOURCE
    src/common/common.cpp
    src/common/color.cpp)

add_library(common STATIC ${COMMON_SOURCE})

set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(common PUBLIC spdlog::spdlog)
target_include_directories(common INTERFACE src/common)

#########################################################
# Core
set(CORE_SOURCE
    src/core/core.cpp
    src/core/scene.cpp
    src/core/bvh.cpp
    src/core/triangle.cpp
    src/core/loader.cpp
    src/core/tgaimage.cpp)

add_library(core STATIC ${CORE_SOURCE})

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(core PUBLIC common)
target_include_directories(core INTERFACE src/core)

#########################################################
# Target

add_executable(ForkerPathTracer src/main/main.cpp)
target_link_libraries(ForkerPathTracer PRIVATE core)

#########################################################

add_executable(Test src/test/test.cpp)
target_link_libraries(Test PRIVATE core)

# Create output directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)
#file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/output)