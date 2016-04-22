add_subdirectory(libjpeg-turbo)
#add_subdirectory(c-blosc-1.8.1)

add_subdirectory(zlib)
MarkAsExternal(zlib)

add_subdirectory(bzip2)
MarkAsExternal(bzip2)

add_subdirectory(lz4)
MarkAsExternal(lz4)

add_subdirectory(lzma)
MarkAsExternal(lzma)