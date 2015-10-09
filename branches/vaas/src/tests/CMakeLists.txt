include(GTest)


option(TRINITY_TEST_LOGGING "Enable log output for tests (slows down test execution)" off)

ExtFolder(gmock)
ExtFolder(gmock_main)
ExtFolder(gtest)
ExtFolder(gtest_main)


file(GLOB TRINITY_TEST_HEADER ${CMAKE_CURRENT_LIST_DIR}/base/*.h
    #${CMAKE_CURRENT_LIST_DIR}/jsonrpc/*.h
    ${CMAKE_CURRENT_LIST_DIR}/common/*.h
	${CMAKE_CURRENT_LIST_DIR}/envtypes/*.h
    ${CMAKE_CURRENT_LIST_DIR}/protocol/*.h
	${CMAKE_CURRENT_LIST_DIR}/gate/*.h
	${CMAKE_CURRENT_LIST_DIR}/ts/*.h
	${CMAKE_CURRENT_LIST_DIR}/testutil/*.h)
file(GLOB TRINITY_TEST_SOURCE ${CMAKE_CURRENT_LIST_DIR}/base/*.cpp
    #${CMAKE_CURRENT_LIST_DIR}/jsonrpc/*.cpp
    )

CreateSourceGroups("${TRINITY_TEST_HEADER}" ${CMAKE_CURRENT_LIST_DIR})
CreateSourceGroups("${TRINITY_TEST_SOURCE}" ${CMAKE_CURRENT_LIST_DIR})

# enter object libraries here
add_executable(tests ${TRINITY_TEST_HEADER} ${TRINITY_TEST_SOURCE}
                    #$<TARGET_OBJECTS:envtypes>
                     $<TARGET_OBJECTS:mocca>
					 ${CMAKE_CURRENT_LIST_DIR}/main.cpp)					 
target_link_libraries(tests gmock)
if (${TRINITY_TEST_TCP})
	set_target_properties(tests PROPERTIES COMPILE_DEFINITIONS "TRINITY_TEST_TCP")
endif ()
if (${TRINITY_TEST_LOGGING})
	set_target_properties(tests PROPERTIES COMPILE_DEFINITIONS "TRINITY_TEST_LOGGING")
endif ()