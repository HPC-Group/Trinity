file(GLOB IO_SOURCE *.c *.cpp
                UVF/*.cpp
                UVF/ExtendedOctree/*.cpp
                Images/*.cpp
                Converter/*.cpp
                GeoConverter/*.cpp
                Service/*.cpp
                DICOM/*.cpp
 		GeometryIO/*.cpp
                VolumeIO/*.cpp)
file(GLOB IO_HEADER *.h
                UVF/*.h
                UVF/ExtendedOctree/*.h
                UVF/ExtendedOctree/*.inc
                Images/*.h
                Converter/*.h
                GeoConverter/*.h
                Service/*.h
                DICOM/*.h
                exception/*.h
		GeometryIO/*.h
                VolumeIO/*.h)

add_definitions(-DTUVOK_NO_QT)


option(EXPRESSIONS "Activate this option if you want to compile the expressions library... this will need flex + bison" OFF)
if(EXPRESSIONS)
    add_subdirectory(expressions)
    ADD_DEFINITIONS(-DCOMPILE_EXPRESSIONS)
endif(EXPRESSIONS)

add_library (IO ${IO_SOURCE} ${IO_HEADER})

find_package(zlib QUIET)
if(ZLIB_FOUND)
else(ZLIB_FOUND)
	set(ZLIB_INCLUDE_DIRS	${CMAKE_BINARY_DIR}/ext/zlib 
							${CMAKE_SOURCE_DIR}/ext/zlib)
	set(ZLIB_LIBRARIES zlibstatic)
endif(ZLIB_FOUND)

target_include_directories (IO PUBLIC ${ZLIB_INCLUDE_DIRS})
target_link_libraries (IO LINK_PUBLIC ${ZLIB_LIBRARIES})

target_include_directories (IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries (IO LINK_PUBLIC Core)
target_link_libraries (IO LINK_PUBLIC Tools)
target_link_libraries (IO LINK_PUBLIC jpeglib)
target_link_libraries (IO LINK_PUBLIC bzip2)
target_link_libraries (IO LINK_PUBLIC tiff_static)
target_link_libraries (IO LINK_PUBLIC lzma)
target_link_libraries (IO LINK_PUBLIC lz4)

if(WIN32)
    target_link_libraries (IO LINK_PUBLIC Shlwapi)
endif(WIN32)

if(EXPRESSIONS)
    target_link_libraries (tuvokIO LINK_PUBLIC expressions)
endif(EXPRESSIONS)
