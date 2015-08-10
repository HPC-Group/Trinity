file(GLOB JPEG_SOURCE *.c)
file(GLOB JPEG_HEADER *.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library (jpeglib ${JPEG_SOURCE} ${JPEG_HEADER})

target_include_directories (jpeglib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
