include(GTest)


option(TRINITY_TEST_LOGGING "Enable log output for tests (slows down test execution)" off)

MarkAsExternal(gmock)
MarkAsExternal(gmock_main)
MarkAsExternal(gtest)
MarkAsExternal(gtest_main)


file(GLOB TRINITY_TEST_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB TRINITY_TEST_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

CreateSourceGroups("${TRINITY_TEST_HEADER}" ${CMAKE_CURRENT_LIST_DIR})
CreateSourceGroups("${TRINITY_TEST_SOURCE}" ${CMAKE_CURRENT_LIST_DIR})

# enter object libraries here
add_executable(tests ${TRINITY_TEST_HEADER} ${TRINITY_TEST_SOURCE}
                     $<TARGET_OBJECTS:frontendbaselib>
                     $<TARGET_OBJECTS:processingbaselib>
                     $<TARGET_OBJECTS:commands>
                     $<TARGET_OBJECTS:common>
                     $<TARGET_OBJECTS:iobaselib>
                     $<TARGET_OBJECTS:mocca>
					 $<TARGET_OBJECTS:silverbullet>
					 $<TARGET_OBJECTS:tripeg>
					 $<TARGET_OBJECTS:opengl-base>
					 ${CMAKE_CURRENT_LIST_DIR}/main.cpp)	

target_link_libraries(tests gmock)
target_link_libraries(tests turbojpeg-static)
TARGET_LINK_LIBRARIES(tests lzma)
TARGET_LINK_LIBRARIES(tests bzip2)
TARGET_LINK_LIBRARIES(tests lz4)
TARGET_LINK_LIBRARIES(tests zlib)
TARGET_LINK_LIBRARIES(tests blosc_static)

TARGET_LINK_LIBRARIES(tests ${OPENGL_LIBRARIES})
IF (UNIX AND NOT APPLE)
	TARGET_LINK_LIBRARIES(tests EGL ${CMAKE_DL_LIBS})
ENDIF()

IF (WIN32)
	TARGET_LINK_LIBRARIES(tests Shlwapi.lib)
ENDIF ()


if (${TRINITY_TEST_LOGGING})
	set_target_properties(tests PROPERTIES COMPILE_DEFINITIONS "TRINITY_TEST_LOGGING")
endif ()
