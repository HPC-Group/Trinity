file(GLOB LZ4_SOURCE *.c)
file(GLOB LZ4_HEADER include/lz4/*.h)

include_directories(include/lz4)
add_library (lz4 ${LZ4_SOURCE} ${LZ4_HEADER})

target_include_directories (lz4 PUBLIC include)
