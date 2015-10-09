cmake_minimum_required(VERSION 2.8.11)
project(vaas)
message(STATUS "generating trinity files")

# seting paths
set(TRINITY_ROOT_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
set(CMAKE_PLATFORM_PATH ${PROJECT_SOURCE_DIR}/cmake/platform)
set(CMAKE_INSTALL_PREFIX ${TRINITY_ROOT_DIR}/build/install)


find_package(OpenGL REQUIRED)


add_definitions(-DTRINITY_VERSION="0.1")
add_definitions(-DTRINITY_ROOT_DIR="${TRINITY_ROOT_DIR}")

add_subdirectory(src)
add_subdirectory(ext)