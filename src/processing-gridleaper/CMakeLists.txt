file(GLOB PROCESSING_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h
							${CMAKE_CURRENT_LIST_DIR}/opengl/*.h
							${CMAKE_CURRENT_LIST_DIR}/opengl/glcore/*.h
							${CMAKE_CURRENT_LIST_DIR}/opengl/glvolume/*.h
							${CMAKE_CURRENT_LIST_DIR}/remote/*.h
							${CMAKE_CURRENT_LIST_DIR}/context/*.h
							${CMAKE_CURRENT_LIST_DIR}/service/*.h
	)

FILE(GLOB PROCESSING_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp
							${CMAKE_CURRENT_LIST_DIR}/*.cpp
							${CMAKE_CURRENT_LIST_DIR}/opengl/*.cpp
							${CMAKE_CURRENT_LIST_DIR}/opengl/glcore/*.cpp
							${CMAKE_CURRENT_LIST_DIR}/opengl/glvolume/*.cpp
							${CMAKE_CURRENT_LIST_DIR}/remote/*.cpp
							${CMAKE_CURRENT_LIST_DIR}/context/*.cpp
							${CMAKE_CURRENT_LIST_DIR}/service/*.cpp
	)


file(GLOB PROCESSING_SHADERS ${CMAKE_CURRENT_LIST_DIR}/shader/*.glsl)

add_definitions(-DGLEW_STATIC)

add_library (processing ${PROCESSING_SOURCE} ${PROCESSING_HEADER} ${PROCESSING_SHADERS} 
						$<TARGET_OBJECTS:mocca>
						$<TARGET_OBJECTS:silverbullet>
			)

#target_include_directories (Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
#target_include_directories (Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shader)

#target_link_libraries (processing LINK_PUBLIC Core)
#target_link_libraries (processing LINK_PUBLIC IO)
target_link_libraries (processing LINK_PUBLIC glew)
target_link_libraries (processing LINK_PUBLIC glfw ${GLFW_LIBRARIES})