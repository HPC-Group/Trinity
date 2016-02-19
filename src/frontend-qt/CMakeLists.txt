file(GLOB TRINITYFRONTEND_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB TRINITYFRONTEND_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h)


CreateSourceGroups("${TRINITYFRONTEND_HEADER}" ${CMAKE_CURRENT_LIST_DIR})
CreateSourceGroups("${TRINITYFRONTEND_SOURCE}" ${CMAKE_CURRENT_LIST_DIR})


SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)  
find_package(OpenGL REQUIRED)

QT5_WRAP_UI(UIS_HDRS window.ui)

add_executable(qtfrontend 
			${TRINITYFRONTEND_SOURCE} 
			${TRINITYFRONTEND_HEADER} 
			${UIS_HDRS}
			$<TARGET_OBJECTS:mocca>
			$<TARGET_OBJECTS:common>
			$<TARGET_OBJECTS:commands>
			$<TARGET_OBJECTS:silverbullet>
			$<TARGET_OBJECTS:frontendbaselib>)

TARGET_LINK_LIBRARIES(qtfrontend Qt5::Widgets)
TARGET_LINK_LIBRARIES(qtfrontend Qt5::OpenGL)

IF (WIN32)
	TARGET_LINK_LIBRARIES(qtfrontend Shlwapi.lib)
ENDIF ()

#qt5_use_modules(qtfrontend Widgets OpenGL)

SET_TARGET_PROPERTIES(qtfrontend PROPERTIES FOLDER "trinity")