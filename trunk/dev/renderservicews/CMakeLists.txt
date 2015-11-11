file(GLOB RENDERSERVICEWS_SOURCE *.cpp)
file(GLOB RENDERSERVICEWS_HEADER *.h)

add_executable(RenderserviceWS ${RENDERSERVICEWS_SOURCE} ${RENDERSERVICEWS_HEADER})

set_target_properties( RenderserviceWS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TRINITY_ROOT_DIR}/build/bin )

include_directories(../src/Externals/glm)
target_link_libraries(RenderserviceWS LINK_PUBLIC Renderer)
target_link_libraries(RenderserviceWS LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries(RenderserviceWS LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(RenderserviceWS LINK_PUBLIC Core)
target_link_libraries(RenderserviceWS LINK_PUBLIC IO)
target_link_libraries(RenderserviceWS LINK_PUBLIC ghoul)

#GHOUL
include_directories(
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)

#MOCCA
include_directories(
					${TRINITY_ROOT_DIR}/src/mocca/include/
					${TRINITY_ROOT_DIR}/src/mocca/include/mocca/)

