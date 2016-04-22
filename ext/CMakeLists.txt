add_subdirectory(libjpeg-turbo)
MarkAsExternal(libjpeg-turbo)

add_subdirectory(zlib)
MarkAsExternal(zlib)
SET(ZLIB_INCLUDE_DIR ${CMAKE_CUURENT_LIST_DIR}/zlib)
SET(ZLIB_LIBRARY $<TARGET_FILE:zlib>)

add_subdirectory(bzip2)
MarkAsExternal(bzip2)
SET(BZIP2_INCLUDE_DIR ${CMAKE_CUURENT_LIST_DIR}/bzip2)
SET(BZIP2_LIBRARY $<TARGET_FILE:bzip2>)

add_subdirectory(lz4)
MarkAsExternal(lz4)
SET(LZ4_INCLUDE_DIR ${CMAKE_CUURENT_LIST_DIR}/lz4)
SET(LZ4_LIBRARY $<TARGET_FILE:lz4>)

add_subdirectory(lzma)
MarkAsExternal(lzma)
SET(LZMA_INCLUDE_DIR ${CMAKE_CUURENT_LIST_DIR}/lzma)
SET(LZMA_LIBRARY $<TARGET_FILE:lzma>)

add_subdirectory(blosc)
MarkAsExternal(blosc_static)
