include(PlatformSpecific)
include(Macros)

set_property(GLOBAL PROPERTY USE_FOLDERS on)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include)
include_directories(${PROJECT_SOURCE_DIR}/src/silverbullet/include)
include_directories(${POCO_INCLUDE_DIR})

# level 0 modules
add_subdirectory(mocca)
add_subdirectory(common)
add_subdirectory(thirdparty)
add_subdirectory(silverbullet)
add_subdirectory(io-fractal)
add_subdirectory(io-base)
add_subdirectory(frontend-base)
add_subdirectory(processing-base)


option(TRINITY_BUILD_TESTS "Build tests for trinity" on)
if (TRINITY_BUILD_TESTS)
    add_subdirectory(tests)
endif()