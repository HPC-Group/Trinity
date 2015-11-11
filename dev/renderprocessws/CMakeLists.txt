file(GLOB RENDERPROCESSWS_SOURCE *.cpp)
file(GLOB RENDERPROCESSWS_HEADER *.h)

file(GLOB REND_SHADERS ${CMAKE_SOURCE_DIR}/src/renderer/Shader/*.glsl)
file(COPY ${REND_SHADERS} DESTINATION ${TRINITY_ROOT_DIR}/build/bin/Debug/Shaders)
file(COPY ${REND_SHADERS} DESTINATION ${TRINITY_ROOT_DIR}/build/bin/Release/Shaders)

add_executable(RenderprocessWS ${RENDERPROCESSWS_SOURCE} ${RENDERPROCESSWS_HEADER})

set_target_properties( RenderprocessWS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TRINITY_ROOT_DIR}/build/bin )

include_directories(../src/Externals/glm)
target_link_libraries(RenderprocessWS LINK_PUBLIC Renderer)
target_link_libraries(RenderprocessWS LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries(RenderprocessWS LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(RenderprocessWS LINK_PUBLIC Core)
target_link_libraries(RenderprocessWS LINK_PUBLIC IO)
target_link_libraries(RenderprocessWS LINK_PUBLIC ghoul)

#GHOUL
include_directories(
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)

#MOCCA
include_directories(
					${TRINITY_ROOT_DIR}/src/mocca/include/
					${TRINITY_ROOT_DIR}/src/mocca/include/mocca/)

