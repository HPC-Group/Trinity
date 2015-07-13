add_subdirectory(renderdemo)
OPTION(BuildQT "Build QT Project" OFF)

IF(BuildQT)
	add_subdirectory(QtFrontend)
ENDIF()