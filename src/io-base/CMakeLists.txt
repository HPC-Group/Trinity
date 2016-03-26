FILE(GLOB IOBASE_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h
						${CMAKE_CURRENT_LIST_DIR}/fractal/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/bzip2/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/lz4/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/lzma/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/zlib/*.h
						${CMAKE_CURRENT_LIST_DIR}/uvf/*.h
)

FILE(GLOB IOBASE_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/fractal/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/*.cpp
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/bzip2/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/lz4/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/lzma/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/UVF-File/ExtendedOctree/ext/zlib/*.c
						${CMAKE_CURRENT_LIST_DIR}/uvf/*.cpp
)


CreateSourceGroups("${IOBASE_HEADER}" ${CMAKE_CURRENT_LIST_DIR})
CreateSourceGroups("${IOBASE_SOURCE}" ${CMAKE_CURRENT_LIST_DIR})

list(REMOVE_ITEM IOBASE_SOURCE ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_library(iobaselib OBJECT ${IOBASE_HEADER} ${IOBASE_SOURCE})

add_executable(iobase 
				${CMAKE_CURRENT_LIST_DIR}/main.cpp
				${IOBASE_HEADER} 
				${IOBASE_SOURCE}
				$<TARGET_OBJECTS:common>
				$<TARGET_OBJECTS:commands>
				$<TARGET_OBJECTS:mocca>
				$<TARGET_OBJECTS:silverbullet>
				$<TARGET_OBJECTS:jsoncpp>)

IF (WIN32)
	TARGET_LINK_LIBRARIES(iobase Shlwapi.lib)
ENDIF ()
				
SET_TARGET_PROPERTIES(iobase PROPERTIES FOLDER "trinity")
SET_TARGET_PROPERTIES(iobaselib PROPERTIES FOLDER "lib")
