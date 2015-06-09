file(GLOB TOOL_SOURCE *.cpp)
file(GLOB TOOL_HEADER *.h)

add_library (Tools ${TOOL_SOURCE} ${TOOL_HEADER})

# Make sure the compiler can find include files for our library
# when other libraries or executables link to Hello
target_include_directories (Tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_subdirectory(DebugOut)

target_link_libraries (Tools LINK_PUBLIC DebugOut)
