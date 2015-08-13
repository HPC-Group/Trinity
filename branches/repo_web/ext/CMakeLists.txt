find_package(zlib QUIET)
if(ZLIB_FOUND)
else(ZLIB_FOUND)
	add_subdirectory(zlib)
endif(ZLIB_FOUND)

add_subdirectory(lz4-new)
#add_subdirectory(lz4-rev88)
add_subdirectory(glew)
add_subdirectory(glfw-3.1)
add_subdirectory(glm)
add_subdirectory(jpeglib)
add_subdirectory(bzip2)
add_subdirectory(lzma)

#add_subdirectory(tiff)
add_subdirectory(libtiff-cmake)
target_include_directories(tiff PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libtiff-cmake)
target_include_directories(tiff PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libtiff-cmake/headers)
target_include_directories(tiff_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libtiff-cmake)
target_include_directories(tiff_static PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libtiff-cmake/headers)

target_include_directories(glfw PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.1/include>
    $<INSTALL_INTERFACE:glfw-3.1/include>  # <prefix>/include/mylib
)


macro(ext_folder EXT_TARGET)
	if(TARGET ${EXT_TARGET})
		set_target_properties(${EXT_TARGET} PROPERTIES FOLDER "Externals")
	endif()
endmacro(ext_folder)

ext_folder(bzip2)
ext_folder(glew)
ext_folder(jpeglib)
ext_folder(lz4)
ext_folder(lzma)
ext_folder(minigzip)
ext_folder(port)
ext_folder(tiff)
ext_folder(tiff_static)
ext_folder(zlib)
ext_folder(zlibstatic)
ext_folder(uninstall) #from glfw
ext_folder(example) #from zlib
