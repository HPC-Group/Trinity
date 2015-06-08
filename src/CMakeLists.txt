set(SOURCE_ROOT_DIR ${TRINITY_ROOT_DIR}/src)
# for now, we will keep our headers at the same location
set(HEADER_ROOT_DIR ${TRINITY_ROOT_DIR}/src)



########## Tests +++++++++++>

# then we could add some tests
if (TRINITY_HAVE_TESTS)

file(GLOB TEST_SOURCE 	${SOURCE_ROOT_DIR}/tests/*cpp )
file(GLOB TEST_HEADER 	${HEADER_ROOT_DIR}/tests/*.h )

set(TEST_DEPENDENCIES 	${TEST_DEPENDENCIES} 
						gtest)

add_executable(testsuite	${TEST_SOURCE} 
							${TEST_HEADER} 
							${TS_SOURCE})


#set_target_properties(testsuite PROPERTIES COMPILE_DEFINITIONS "GATE_STATIC")
target_link_libraries(testsuite ${TEST_DEPENDENCIES} gtest gmock gtest_main)
add_test( testsuite testsuite )  
install(TARGETS testsuite DESTINATION bin)
endif ()