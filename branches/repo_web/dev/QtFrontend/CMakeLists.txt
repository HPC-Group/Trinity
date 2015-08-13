cmake_minimum_required(VERSION 2.8.11)

file(GLOB QTFRONTEND_SOURCE *.cpp)
file(GLOB QTFRONTEND_HEADER *.h)

file(GLOB REND_SHADERS ${CMAKE_SOURCE_DIR}/src/renderer/Shader/*.glsl)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

#set (CMAKE_LIBRARY_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")
#set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.3.0\\5.3\\msvc2013_64_opengl\\")

project(QtFrontend)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

QT5_WRAP_UI(UIS_HDRS qtfrontendwindow.ui)

add_executable(QtFrontend ${QTFRONTEND_SOURCE} ${QTFRONTEND_HEADER} ${UIS_HDRS})

include_directories(../src/Externals/glm)
target_link_libraries(QtFrontend LINK_PUBLIC Qt5::Widgets)
target_link_libraries(QtFrontend LINK_PUBLIC Renderer)
target_link_libraries(QtFrontend LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries(QtFrontend LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(QtFrontend LINK_PUBLIC Core)
target_link_libraries(QtFrontend LINK_PUBLIC IO)
target_link_libraries(QtFrontend LINK_PUBLIC ghoul)

#GHOUL
include_directories(
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)






