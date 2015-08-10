file(GLOB LZMA_SOURCE *.c)
file(GLOB LZMA_HEADER *.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library (lzma ${LZMA_SOURCE} ${LZMA_HEADER})

target_include_directories (lzma PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
