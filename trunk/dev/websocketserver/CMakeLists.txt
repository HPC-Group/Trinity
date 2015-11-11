file(GLOB DEMOREN2_SOURCE *.cpp)
file(GLOB DEMOREN2_HEADER *.h)

file(GLOB REND_SHADERS ${CMAKE_SOURCE_DIR}/src/renderer/Shader/*.glsl)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

add_executable(websocketserver ${DEMOREN2_SOURCE} ${DEMOREN2_HEADER})

set_target_properties( websocketserver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TRINITY_ROOT_DIR}/build/bin )

include_directories(../src/Externals/glm)
target_link_libraries(websocketserver LINK_PUBLIC Renderer)
target_link_libraries(websocketserver LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries(websocketserver LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(websocketserver LINK_PUBLIC Core)
target_link_libraries(websocketserver LINK_PUBLIC IO)
target_link_libraries(websocketserver LINK_PUBLIC ghoul)

target_link_libraries(websocketserver LINK_PUBLIC pushframework)
target_link_libraries(websocketserver LINK_PUBLIC websocketprotocol)

include_directories(
					${TRINITY_ROOT_DIR}/src/pushframework/include/
					${TRINITY_ROOT_DIR}/src/websocketprotocol/include/)

#GHOUL
include_directories(
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)

#MOCCA
include_directories(
					${TRINITY_ROOT_DIR}/src/mocca/include/
					${TRINITY_ROOT_DIR}/src/mocca/include/mocca/)

