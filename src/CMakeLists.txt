include(PlatformSpecific)
include(Macros)

set_property(GLOBAL PROPERTY USE_FOLDERS on)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include/opengl-base)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include/opengl-base/GL)
include_directories(${PROJECT_SOURCE_DIR}/src/silverbullet/include)

# level 0 modules
add_subdirectory(mocca)
add_subdirectory(commands)
add_subdirectory(common)
add_subdirectory(silverbullet)
add_subdirectory(io-base)
add_subdirectory(thirdparty)



option(TRINITY_BUILD_STUFF "Build real useful stuff for trinity" off)

if (TRINITY_BUILD_STUFF)
    add_subdirectory(frontend-qt)
    add_subdirectory(frontend-base)
	add_subdirectory(opengl-base)
	add_subdirectory(processing-base)
endif()



option(TRINITY_BUILD_TESTS "Build tests for trinity" off)
if (TRINITY_BUILD_TESTS AND TRINITY_BUILD_STUFF)
    add_subdirectory(tests)
endif()