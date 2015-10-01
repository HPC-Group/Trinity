cmake_minimum_required(VERSION 2.8.11)

file(GLOB QTFRONTEND_SOURCE *.cpp)
file(GLOB QTFRONTEND_HEADER *.h)

file(GLOB REND_SHADERS ${CMAKE_SOURCE_DIR}/src/renderer/Shader/*.glsl)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${REND_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

file(GLOB MUI_GATE_DLL ${CMAKE_SOURCE_DIR}/dev/QtFrontend/gate_d.dll)
file(GLOB MUI_GATE_LIB ${CMAKE_SOURCE_DIR}/dev/QtFrontend/gate_d.lib)
file(COPY ${MUI_GATE_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Lib)
file(COPY ${MUI_GATE_LIB} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Lib)

project(QtFrontend)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

QT5_WRAP_UI(UIS_HDRS qtfrontendwindow.ui)

add_executable(QtFrontend ${QTFRONTEND_SOURCE} ${QTFRONTEND_HEADER} ${UIS_HDRS} $<TARGET_OBJECTS:mocca>)

include_directories(../src/ext/glm)

include_directories(
					${TRINITY_ROOT_DIR}/src/
					${TRINITY_ROOT_DIR}/ext/glew/include/
					${TRINITY_ROOT_DIR}/src/mocca/include/
					${TRINITY_ROOT_DIR}/src/mocca/include/mocca/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/
					${TRINITY_ROOT_DIR}/ext/ghoul/include/ghoul/)

target_link_libraries(QtFrontend LINK_PUBLIC Qt5::Widgets)
target_link_libraries(QtFrontend LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(QtFrontend LINK_PUBLIC ghoul)
target_link_libraries(QtFrontend LINK_PUBLIC jpeglib)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/Lib)

target_link_libraries (QtFrontend Lib/gate_d)






