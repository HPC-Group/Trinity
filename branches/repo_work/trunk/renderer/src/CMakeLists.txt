CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(TrinityRenderer)

###############################################################################
# Don't allow the user to build in the src directory...
#

IF(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
  MESSAGE( "" )
  MESSAGE( "Building SCIRun in the source directory is not allowed." )
  MESSAGE( "(In order to clean up, you now need to run: rm -rf CMakeFiles CMakeCache.txt" )
  MESSAGE( "" )
  MESSAGE( FATAL_ERROR "Goodbye." )
ENDIF()

########################################################################
# Build shared libraries by default

OPTION(BUILD_SHARED_LIBS "Build shared libraries" ON)
MARK_AS_ADVANCED(BUILD_SHARED_LIBS)

########################################################################
# Set default CMAKE_BUILD_TYPE
# if empty for Unix Makefile builds

IF (CMAKE_GENERATOR MATCHES "Unix Makefiles" AND NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF (CMAKE_GENERATOR MATCHES "Unix Makefiles" AND NOT CMAKE_BUILD_TYPE)

########################################################################
# APPLICATION Information (used by CPack)
#

SET(APPLICATION_NAME "TrinityRenderer" CACHE INTERNAL "Application name" FORCE)
# TODO: temporary hack to allow testing
SET(TEST_APPLICATION_NAME "TrinityRenderer_test" CACHE INTERNAL "Application name" FORCE)

########################################################################
# Standard system and compiler tests

INCLUDE(CMakeBackwardCompatibilityC)
INCLUDE(CMakeBackwardCompatibilityCXX)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckCXXCompilerFlag)
INCLUDE(ExternalProject)

IF(WIN32)
  ADD_DEFINITIONS("/wd4251 /wd4275 -D_CRT_SECURE_NO_WARNINGS")
ENDIF(WIN32)

########################################################################
# External project utilities

OPTION(DISABLE_ExternalProject_UPDATE "Turn off ExternalProject UPDATE_COMMAND property (if not connected to the internet, for example" ON)
MARK_AS_ADVANCED(DISABLE_ExternalProject_UPDATE)
IF(DISABLE_ExternalProject_UPDATE)
  # do nothing commands...
  IF(WIN32)
    SET(DISABLED_UPDATE "UPDATE_COMMAND" "rem")
  ELSE()
    SET(DISABLED_UPDATE "UPDATE_COMMAND" "echo")
  ENDIF()
ENDIF()

########################################################################
# Configure test support
# Support for gtest

IF(TRINITY_HAVE_TESTS)
  SET(gtest_force_shared_crt ON CACHE BOOL  "Always use msvcrt.dll" FORCE)
  ADD_SUBDIRECTORY(../../ext/gmock) #Build gmock
  ENABLE_TESTING()
  INCLUDE_DIRECTORIES(${GMOCK_INLCLUDE_DIR}/include ${GMOCK_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${GTEST_INLCLUDE_DIR}/include ${GTEST_INLCUDE_DIR})
  IF(WIN32)
    SET_TARGET_PROPERTIES(gtest gtest_main  PROPERTIES COMPILE_DEFINITIONS "_VARIADIC_MAX=10")
  ENDIF()
ENDIF()

########################################################################
# Custom compiler flags and definitions

# TODO: Better compiler check for OS X?
# Try CMAKE_<LANG>_COMPILER_ID MATCHES Clang? Or check for Intel, GNU?
IF(UNIX)
  SET(CXX_LANGUAGE_STANDARD_FLAG "-std=c++11")
  CHECK_CXX_COMPILER_FLAG(${CXX_LANGUAGE_STANDARD_FLAG} USE_CXX11_FLAG)
  IF(NOT USE_CXX11_FLAG)
    SET(CXX_LANGUAGE_STANDARD_FLAG "-std=c++0x")
    CHECK_CXX_COMPILER_FLAG(${CXX_LANGUAGE_STANDARD_FLAG} USE_CXX0x_FLAG)
    IF(NOT USE_CXX0x_FLAG)
      SET(CXX_LANGUAGE_STANDARD_FLAG "")
    ENDIF()
  ENDIF()

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_LANGUAGE_STANDARD_FLAG} -Wall")
  IF(NOT APPLE)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed -ldl -lrt")
  ENDIF()
ENDIF()

IF(APPLE)
  SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
  SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -ftemplate-depth=256")
  SET(CMAKE_CXX_FLAGS_DEBUG "-Wshorten-64-to-32 ${CMAKE_CXX_FLAGS_DEBUG}")

  ADD_DEFINITIONS(-DGTEST_USE_OWN_TR1_TUPLE=1)
ENDIF()

########################################################################
# Find and configure OpenGL

FIND_PACKAGE(OpenGL REQUIRED)

########################################################################
# Includes

SET(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${TrinityRenderer_SOURCE_DIR}/
  ${TrinityRenderer_SOURCE_DIR}/../..
  ${TrinityRenderer_SOURCE_DIR}/Shader
  ${TrinityRenderer_SOURCE_DIR}/../../ext/ghoul/include/
  ${TrinityRenderer_SOURCE_DIR}/../../ext/ghoul/include/ghoul)

FILE(GLOB GHOUL_SOURCE ${TrinityRenderer_SOURCE_DIR}/../../ext/ghoul/src/logging/*.cpp 
	${TrinityRenderer_SOURCE_DIR}/ext/ghoul/src/cmdparser/*.cpp)

FILE(GLOB GHOUL_HEADER ${TrinityRenderer_SOURCE_ROOT_DIR}/ext/ghoul/include/ghoul/logging/*.h 
	${TrinityRenderer_SOURCE_DIR}/ext/ghoul/include/ghoul/cmdparser/*.h)

ADD_LIBRARY(ghoul  ${GHOUL_SOURCE} ${GHOUL_HEADER})

SET(EXTERNAL_SOURCE_DIR ${TrinityRenderer_SOURCE_DIR}/../../ext)


file(GLOB TrinityRenderer_SOURCE *.cpp
  OpenGL/*.cpp
  OpenGL/GLCore/*.cpp
  OpenGL/GLVolume/*.cpp
  Context/*.cpp
  Service/*.cpp)

file(GLOB TrinityRenderer_HEADER *.h
  OpenGL/*.h
  OpenGL/GLCore/*.h
  OpenGL/GLVolume/*.h
  Context/*.h
  Service/*.h) 

file(GLOB TrinityRenderer_SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/Shader/*.glsl)

add_definitions(-DGLEW_STATIC)
add_library (TrinityRenderer ${TrinityRenderer_SOURCE} ${TrinityRenderer_HEADER} ${TrinityRenderer_SHADERS})

target_include_directories (TrinityRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories (TrinityRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shader)

target_link_libraries (TrinityRenderer LINK_PUBLIC ../../ext/Tools)
target_link_libraries (TrinityRenderer LINK_PUBLIC ../../ext/Core)
target_link_libraries (TrinityRenderer LINK_PUBLIC ../../ect/IO)
target_link_libraries (TrinityRenderer LINK_PUBLIC glew)
target_link_libraries (TrinityRenderer LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries (TrinityRenderer LINK_PUBLIC ghoul)
