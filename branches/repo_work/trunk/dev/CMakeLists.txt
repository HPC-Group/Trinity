add_subdirectory(renderdemo)
add_subdirectory(renderdemo_network)
add_subdirectory(renderdemo_client)
OPTION(BuildQT "Build QT Project" OFF)

IF(BuildQT)
	add_subdirectory(QtFrontend)
ENDIF()
