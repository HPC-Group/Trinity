file(GLOB BZIP_SOURCE *.c)
file(GLOB BZIP_HEADER *.h)

add_library (bzip2 ${BZIP_SOURCE} ${BZIP_HEADER})

target_include_directories (bzip2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
