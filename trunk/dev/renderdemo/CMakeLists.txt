file(GLOB DEMOREN2_SOURCE *.cpp)
file(GLOB DEMOREN2_HEADER *.h)

file(GLOB REND_SHADERS ${CMAKE_SOURCE_DIR}/src/renderer/Shader/*.glsl)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

add_executable(RenderDemo ${DEMOREN2_SOURCE} ${DEMOREN2_HEADER})

include_directories(../src/Externals/glm)
target_link_libraries(RenderDemo LINK_PUBLIC Renderer)
target_link_libraries(RenderDemo LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries(RenderDemo LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(RenderDemo LINK_PUBLIC Core)
target_link_libraries(RenderDemo LINK_PUBLIC IO)
target_link_libraries(RenderDemo LINK_PUBLIC ghoul)

#GHOUL
include_directories(
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)

