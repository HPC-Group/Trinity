add_subdirectory(renderdemo)
add_subdirectory(renderprocess)
add_subdirectory(renderservice)
OPTION(BuildQTMichael "Build QT Project" OFF)
OPTION(BuildQTNetwork "Build QT Project" OFF)

IF(BuildQTMichael)
	add_subdirectory(QtFrontend)
ENDIF()

IF(BuildQTNetwork)
	add_subdirectory(QtNetwork_client)
ENDIF()
