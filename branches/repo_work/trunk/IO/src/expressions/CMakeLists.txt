file(GLOB EXPRESSION_SOURCE *.cpp)
file(GLOB EXPRESSION_HEADER *.h)

add_library (expressions ${IO_SOURCE} ${IO_HEADER})

target_include_directories (tuvokIO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)