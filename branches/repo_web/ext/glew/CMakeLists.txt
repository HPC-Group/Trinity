file(GLOB GLEW_SOURCE src/glew.c)
file(GLOB GLEW_HEADER include/GL/*.h)

add_definitions(-DGLEW_STATIC)
add_library (glew ${GLEW_SOURCE} ${GLEW_HEADER})

target_include_directories(glew PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)