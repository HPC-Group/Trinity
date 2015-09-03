if (WIN32)

ExternalProject_Add(zeromq
  SOURCE_DIR ${MORPHABLEUI_ROOT_DIR}/ext/zeromq
  CONFIGURE_COMMAND ""
  BUILD_COMMAND devenv /upgrade ${MORPHABLEUI_ROOT_DIR}/ext/zeromq/builds/msvc/msvc.sln
        COMMAND msbuild /p:Configuration=StaticRelease
						/t:libzmq
						${MORPHABLEUI_ROOT_DIR}/ext/zeromq/builds/msvc/msvc.sln
  INSTALL_COMMAND ""
)

 ExternalProject_Add_Step(
    zeromq CopyLibrary
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MORPHABLEUI_ROOT_DIR}/ext/zeromq/lib/libzmq.lib ${MORPHABLEUI_ROOT_DIR}/build/zeromq/libzmq.lib
    DEPENDEES install)

endif()