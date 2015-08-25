add_subdirectory(renderdemo)
add_subdirectory(renderdemo_network)
add_subdirectory(renderdemo_client)
OPTION(BuildQTMichael "Build QT Project" OFF)
OPTION(BuildQTNetwork "Build QT Project" OFF)

IF(BuildQT)
	add_subdirectory(QtFrontend)
ENDIF()

IF(BuildQTNetwork)
	add_subdirectory(QtNetwork_client)
ENDIF()
