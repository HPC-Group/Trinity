# Build and set up the tests

include_directories("${TIFF_CONFIG_DIR}" "../libtiff")

# Directory where to run the tests
set(TEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/workspace")
file(MAKE_DIRECTORY "${TEST_DIR}")

# Helper macro to add a test executable and link it to libtiff
macro (add_test_exe exename)
  add_executable(${exename} ${ARGN})
  target_link_libraries(${exename} tiff_static)
  set_target_properties(${exename} PROPERTIES FOLDER "test")
  if (MSVC)
    # Disable POSIX-deprecation and downcasting warnings
    set_target_properties(${exename} PROPERTIES COMPILE_FLAGS "/wd4996 /wd4244 /wd4305")
  endif()
  
  # Add a basic test with the exename
  add_test(NAME "test_${exename}" WORKING_DIRECTORY "${TEST_DIR}"
    COMMAND ${exename})
  
endmacro()

add_test_exe(ascii_tag ascii_tag.c)
add_test_exe(long_tag  long_tag.c check_tag.c)
add_test_exe(short_tag short_tag.c check_tag.c)
add_test_exe(strip_rw  strip_rw.c strip.c test_arrays.c test_arrays.h)
add_test_exe(rewrite   rewrite_tag.c)
