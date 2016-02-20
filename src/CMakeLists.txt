include(PlatformSpecific)
include(Macros)

set_property(GLOBAL PROPERTY USE_FOLDERS on)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/mocca/include)
include_directories(${PROJECT_SOURCE_DIR}/src/silverbullet/include)

# level 0 modules
add_subdirectory(mocca)
add_subdirectory(commands)
add_subdirectory(common)
add_subdirectory(silverbullet)
add_subdirectory(io-base)
add_subdirectory(frontend-base)
add_subdirectory(thirdparty)

add_subdirectory(processing-base)

option(TRINITY_BUILD_QT_FRONTEND "Build qt frontend for trinity" off)
if (TRINITY_BUILD_QT_FRONTEND)
    add_subdirectory(frontend-qt)
endif()

option(TRINITY_BUILD_TESTS "Build tests for trinity" on)
if (TRINITY_BUILD_TESTS)
    add_subdirectory(tests)
endif()