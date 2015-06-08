cmake_minimum_required(VERSION 2.8.4)
project(trinity)

# setting the project root directory
set(TRINITY_ROOT_DIR ${PROJECT_SOURCE_DIR})
add_definitions(-DTRINITY_ROOT_DIR="${TRINITY_ROOT_DIR}")

# install libraries to ~/local if you don't have sudo permissions; fixme dmc: remove when refactoring is finished
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ~/local)

set(CMAKE_INSTALL_PREFIX ${TRINITY_ROOT_DIR}/build/install)
set(CMAKE_MODULE_PATH ${TRINITY_ROOT_DIR}/cmake/modules)
set(CMAKE_PLATFORM_PATH ${TRINITY_ROOT_DIR}/cmake/platform)



#turn on using solution folders
#todo verify if needed
set_property( GLOBAL PROPERTY USE_FOLDERS ON)
option(TRINITY_HAVE_TESTS "Activate the unit tests" ON)

#todo is this really necessary?
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(!WIN32)
        message(STATUS "${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "Setting build type to 'Debug' as none was specified.")
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
   # Set the possible values of build type for cmake-gui
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
     "MinSizeRel" "RelWithDebInfo")
endif()


# support for gtest
if (TRINITY_HAVE_TESTS)
  set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
    add_subdirectory(ext/gmock) #Build gmock
  enable_testing()
  include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  if (WIN32) 
    set_target_properties(gtest gtest_main 
                          PROPERTIES COMPILE_DEFINITIONS "_VARIADIC_MAX=10")
  endif ()
endif ()



find_package(OpenGL REQUIRED)

add_definitions(-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

if(WIN32)
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)
endif(WIN32)

if(UNIX AND NOT APPLE)
    find_package(Threads)
endif(UNIX AND NOT APPLE)

add_subdirectory(src)