file(GLOB TRINITYDEMOFRONTEND_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB TRINITYDEMOFRONTEND_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h)


CreateSourceGroups("${TRINITYDEMOFRONTEND_HEADER}" ${CMAKE_CURRENT_LIST_DIR})
CreateSourceGroups("${TRINITYDEMOFRONTEND_SOURCE}" ${CMAKE_CURRENT_LIST_DIR})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)  
find_package(OpenGL REQUIRED)

QT5_WRAP_UI(UIS_HDRS MainWindow.ui)
QT5_WRAP_UI(UIS_HDRS ConnectionWidget.ui)

add_executable(frontend-qt-waschk 
			${TRINITYDEMOFRONTEND_SOURCE} 
			${TRINITYDEMOFRONTEND_HEADER} 
			${UIS_HDRS}
			$<TARGET_OBJECTS:mocca>
			$<TARGET_OBJECTS:common>
			$<TARGET_OBJECTS:commands>
			$<TARGET_OBJECTS:silverbullet>
			$<TARGET_OBJECTS:frontendbaselib>
		    $<TARGET_OBJECTS:jsoncpp>
		   	$<TARGET_OBJECTS:tripeg>
			$<TARGET_OBJECTS:opengl-base>)

TARGET_LINK_LIBRARIES(frontend-qt-waschk turbojpeg-static)
TARGET_LINK_LIBRARIES(frontend-qt-waschk Qt5::Widgets)
TARGET_LINK_LIBRARIES(frontend-qt-waschk Qt5::OpenGL)
TARGET_LINK_LIBRARIES(frontend-qt-waschk ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(frontend-qt-waschk blosc_static)

IF (WIN32)
	TARGET_LINK_LIBRARIES(frontend-qt-waschk Shlwapi.lib)
ENDIF ()

#qt5_use_modules(frontend-qt-waschk Widgets OpenGL)

file(GLOB UIELEMENTS ${CMAKE_CURRENT_SOURCE_DIR}/stylesheet.css ${CMAKE_CURRENT_SOURCE_DIR}/web_icon.png)
file(COPY ${UIELEMENTS} DESTINATION ${CMAKE_BINARY_DIR}/src/frontend-qt-waschk/)

SET_TARGET_PROPERTIES(frontend-qt-waschk PROPERTIES FOLDER "trinity")