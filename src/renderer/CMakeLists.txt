file(GLOB REND2_SOURCE   *.cpp
						OpenGL/*.cpp
                        OpenGL/GLCore/*.cpp
                        OpenGL/GLVolume/*.cpp
                        Context/*.cpp
						Service/*.cpp)
file(GLOB REND2_HEADER   *.h
                        OpenGL/*.h
                        OpenGL/GLCore/*.h
                        OpenGL/GLVolume/*.h
                        Context/*.h
			Service/*.h)
file(GLOB REND2_SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/Shader/*.glsl)

add_definitions(-DGLEW_STATIC)
add_library (Renderer ${REND2_SOURCE} ${REND2_HEADER} ${REND2_SHADERS})

target_include_directories (Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories (Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shader)

target_link_libraries (Renderer LINK_PUBLIC Tools)
target_link_libraries (Renderer LINK_PUBLIC Core)
target_link_libraries (Renderer LINK_PUBLIC IO)
target_link_libraries (Renderer LINK_PUBLIC glew)
target_link_libraries (Renderer LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries (Renderer LINK_PUBLIC ghoul)

