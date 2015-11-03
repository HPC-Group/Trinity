include(PlatformSpecific)
include(CreateSourceGroups)
include(ExtFolder)

set_property(GLOBAL PROPERTY USE_FOLDERS on)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include)

# level 0 modules
add_subdirectory(mocca)
add_subdirectory(common)
add_subdirectory(thirdparty)
add_subdirectory(silverbullet)
#add_subdirectory(io-fractal)


option(TRINITY_BUILD_TESTS "Build tests for trinity" on)
if (TRINITY_BUILD_TESTS)
    add_subdirectory(tests)
endif()