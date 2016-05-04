FILE(GLOB IOBASE_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h
						${CMAKE_CURRENT_LIST_DIR}/fractal/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/Mesh/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/bzip2/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/lz4/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/lzma/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/zlib/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/*.h
)

FILE(GLOB IOBASE_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/fractal/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/Mesh/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/bzip2/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/lz4/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/lzma/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/Dataset/UVF-File/ExtendedOctree/ext/zlib/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/*.cpp
)


CreateSourceGroups("${IOBASE_HEADER}" ${CMAKE_CURRENT_LIST_DIR})
CreateSourceGroups("${IOBASE_SOURCE}" ${CMAKE_CURRENT_LIST_DIR})

list(REMOVE_ITEM IOBASE_SOURCE ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_library(iobaselib OBJECT ${IOBASE_HEADER} ${IOBASE_SOURCE})

add_executable(iobase 
				${CMAKE_CURRENT_LIST_DIR}/main.cpp
				$<TARGET_OBJECTS:iobaselib>
				$<TARGET_OBJECTS:common>
				$<TARGET_OBJECTS:commands>
				$<TARGET_OBJECTS:mocca>
				$<TARGET_OBJECTS:silverbullet>)

TARGET_LINK_LIBRARIES(iobase lzma)
TARGET_LINK_LIBRARIES(iobase bzip2)
TARGET_LINK_LIBRARIES(iobase lz4)
TARGET_LINK_LIBRARIES(iobase zlib)
TARGET_LINK_LIBRARIES(iobase blosc_static)

IF (WIN32)
	TARGET_LINK_LIBRARIES(iobase Shlwapi.lib)
ENDIF ()
				
SET_TARGET_PROPERTIES(iobase PROPERTIES FOLDER "trinity")
SET_TARGET_PROPERTIES(iobaselib PROPERTIES FOLDER "lib")

SET(TRINITY_IO_INSTALL_PATH io)
INSTALL(TARGETS iobase RUNTIME DESTINATION ${TRINITY_IO_INSTALL_PATH})