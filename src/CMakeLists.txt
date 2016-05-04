include(PlatformSpecific)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/mocca/src)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include/opengl-base)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include/opengl-base/GL)
include_directories(${PROJECT_SOURCE_DIR}/src/silverbullet/include)

# level 0 modules
add_subdirectory(mocca)
add_subdirectory(commands)
add_subdirectory(common)
add_subdirectory(jpeg)
add_subdirectory(silverbullet)
add_subdirectory(io-base)
add_subdirectory(frontend-base)
add_subdirectory(opengl-base)
add_subdirectory(processing-base)
add_subdirectory(combined)


option(TRINITY_BUILD_FRONTEND "Build frontends for trinity" off)

if (TRINITY_BUILD_FRONTEND)
    add_subdirectory(frontend-qt)
	add_subdirectory(frontend-web)
endif()



option(TRINITY_BUILD_TESTS "Build tests for trinity" on)
if (TRINITY_BUILD_TESTS)
    add_subdirectory(tests)
endif()