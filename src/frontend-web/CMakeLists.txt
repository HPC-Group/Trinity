SET(TRINITY_FRONTENT_WEB_INSTALL_PATH frontend-web)
INSTALL(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/ DESTINATION ${TRINITY_FRONTENT_WEB_INSTALL_PATH} PATTERN *CMakeLists.txt EXCLUDE)