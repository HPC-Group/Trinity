cmake_minimum_required(VERSION 2.8.11)

file(GLOB QTFRONTEND_SOURCE *.cpp)
file(GLOB QTFRONTEND_HEADER *.h)

#set (CMAKE_LIBRARY_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")
#set (CMAKE_PREFIX_PATH "C:\\Qt5.5\\msvc2013_64")
set (CMAKE_PREFIX_PATH "/home/waschkbaer/Qt/5.5/gcc_64/lib/cmake/Qt5Widgets")

project(QtFrontend)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

QT5_WRAP_UI(UIS_HDRS client.ui)

add_executable(QtNetwork_Client ${QTFRONTEND_SOURCE} ${QTFRONTEND_HEADER} ${UIS_HDRS} $<TARGET_OBJECTS:mocca>)

set_target_properties( QtNetwork_Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TRINITY_ROOT_DIR}/build/bin )

include_directories(../src/Externals/glm)
target_link_libraries(QtNetwork_Client LINK_PUBLIC Qt5::Widgets)
target_link_libraries(QtNetwork_Client LINK_PUBLIC Renderer)
target_link_libraries(QtNetwork_Client LINK_PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries(QtNetwork_Client LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(QtNetwork_Client LINK_PUBLIC Core)
target_link_libraries(QtNetwork_Client LINK_PUBLIC IO)
target_link_libraries(QtNetwork_Client LINK_PUBLIC ghoul)
target_link_libraries(QtNetwork_Client LINK_PUBLIC lz4)

#GHOUL
include_directories(
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)

#MOCCA
include_directories(
					${TRINITY_ROOT_DIR}/src/mocca/include/
					${TRINITY_ROOT_DIR}/src/mocca/include/mocca/)






