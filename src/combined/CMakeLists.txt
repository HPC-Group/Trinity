add_executable(combined 
				${CMAKE_CURRENT_LIST_DIR}/main.cpp
				$<TARGET_OBJECTS:processingbaselib>
				$<TARGET_OBJECTS:iobaselib>
				$<TARGET_OBJECTS:common>
				$<TARGET_OBJECTS:commands>
				$<TARGET_OBJECTS:mocca>
				$<TARGET_OBJECTS:silverbullet>
				$<TARGET_OBJECTS:tripeg>
				$<TARGET_OBJECTS:opengl-base>)

file(GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/../processing-base/gridleaper/*.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/../processing-base/simplerenderer/*.glsl)
file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/src/combined/)

TARGET_LINK_LIBRARIES(combined ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(combined turbojpeg-static)
TARGET_LINK_LIBRARIES(combined lzma)
TARGET_LINK_LIBRARIES(combined bzip2)
TARGET_LINK_LIBRARIES(combined lz4)
TARGET_LINK_LIBRARIES(combined zlib)
TARGET_LINK_LIBRARIES(combined blosc_static)

IF (UNIX AND NOT APPLE)
	TARGET_LINK_LIBRARIES(combined EGL ${CMAKE_DL_LIBS})
ENDIF()

IF (WIN32)
	TARGET_LINK_LIBRARIES(combined Shlwapi.lib)
ENDIF ()

SET_TARGET_PROPERTIES(combined PROPERTIES FOLDER "trinity")