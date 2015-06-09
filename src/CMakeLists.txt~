set(SOURCE_ROOT_DIR ${TRINITY_ROOT_DIR}/src)
# for now, we will keep our headers at the same location
set(HEADER_ROOT_DIR ${TRINITY_ROOT_DIR}/src)

# global visibility
include_directories(
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)

add_subdirectory(Tools)
add_subdirectory(Renderer)
add_subdirectory(IO)
add_subdirectory(Core)


########## Ghoul Objects +++++++++++>

#file(GLOB GHOUL_SOURCE ${TRINITY_ROOT_DIR}/ext/ghoul/src/logging/*.cpp
#					   ${TRINITY_ROOT_DIR}/ext/ghoul/src/cmdparser/*.cpp)
#file(GLOB GHOUL_HEADER ${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/logging/*.h
#					   ${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/cmdparser/*.h)
#
#add_library(ghoul OBJECT ${GHOUL_SOURCE} ${GHOUL_HEADER})


########## Tests +++++++++++>

# then we could add some tests
if (TRINITY_HAVE_TESTS)

file(GLOB TEST_SOURCE 	${SOURCE_ROOT_DIR}/tests/*cpp )
file(GLOB TEST_HEADER 	${HEADER_ROOT_DIR}/tests/*.h )

set(TEST_DEPENDENCIES 	${TEST_DEPENDENCIES} 
						gtest)

add_executable(testsuite	${TEST_SOURCE} 
							${TEST_HEADER} 
							#$<TARGET_OBJECTS:ghoul>
							)


#set_target_properties(testsuite PROPERTIES COMPILE_DEFINITIONS "GATE_STATIC")
target_link_libraries(testsuite ${TEST_DEPENDENCIES} gtest gmock gtest_main)
add_test( testsuite testsuite )  
install(TARGETS testsuite DESTINATION bin)
endif ()
