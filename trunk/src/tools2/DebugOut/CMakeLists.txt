file(GLOB DEBUGOUT_SOURCE AbstrDebugOut.cpp MultiplexOut.cpp ConsoleOut.cpp)
file(GLOB DEBUGOUT_HEADER *.h)

add_library (DebugOut ${DEBUGOUT_SOURCE})

# Make sure the compiler can find include files for our library
# when other libraries or executables link to Hello
target_include_directories (DebugOut PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
