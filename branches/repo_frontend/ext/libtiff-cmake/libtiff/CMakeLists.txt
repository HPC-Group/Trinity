# Derived from Makefile.am

include_directories(BEFORE "${TIFF_CONFIG_DIR}")
add_definitions(-DHAVE_CONFIG_H)

if (WIN32)
  option(LIBTIFF_WINMODE_WINDOWED
    "Report errors via MessageBox() instead of stderr." OFF)
  if (LIBTIFF_WINMODE_WINDOWED)
    add_definitions(-DTIF_PLATFORM_WINDOWED)
  else()
    add_definitions(-DTIF_PLATFORM_CONSOLE)
  endif()
endif()

set(libtiffinclude_HEADERS
	tiff.h
	tiffio.h
	tiffvers.h
)

if (HAVE_CXX)
  list(APPEND libtiffinclude_HEADERS tiffio.hxx)
endif()

if (EXISTS "tiffconf.h" OR EXISTS "tif_config.h")
  message(FATAL_ERROR "tiffconf.h/tif_config.h already exist in the source directory.")
endif()
set(nodist_libtiffinclude_HEADERS
	"${TIFF_CONFIG_DIR}/tiffconf.h"
)

set(noinst_HEADERS
	t4.h
	tif_dir.h
	tif_predict.h
	tiffiop.h
	uvcode.h
  "${TIFF_CONFIG_DIR}/tif_config.h"
)

set(libtiff_SOURCES
	tif_aux.c
	tif_close.c
	tif_codec.c
	tif_color.c
	tif_compress.c
	tif_dir.c
	tif_dirinfo.c
	tif_dirread.c
	tif_dirwrite.c
	tif_dumpmode.c
	tif_error.c
	tif_extension.c
	tif_fax3.c
	tif_fax3sm.c
	tif_flush.c
	tif_getimage.c
	tif_jbig.c
	tif_jpeg.c
	tif_jpeg_12.c
	tif_luv.c
	tif_lzma.c
	tif_lzw.c
	tif_next.c
	tif_ojpeg.c
	tif_open.c
	tif_packbits.c
	tif_pixarlog.c
	tif_predict.c
	tif_print.c
	tif_read.c
	tif_strip.c
	tif_swab.c
	tif_thunder.c
	tif_tile.c
	tif_version.c
	tif_warning.c
	tif_write.c
	tif_zip.c
)

if (HAVE_CXX)
  list(APPEND libtiff_SOURCES tif_stream.cxx)
endif()

if (WIN32_IO)
  add_definitions(-DUSE_WIN32_FILEIO=1)
  list(APPEND libtiff_SOURCES tif_win32.c)
else()
  list(APPEND libtiff_SOURCES tif_unix.c)
endif()

if (MSVC)
  set(TIFF_DEF_FILE libtiff.def)
  set(TIFF_RESOURCES libtiff.rc)
endif()

# TODO Add the LD_VERSION_SCRIPT files

# Create the targets
set(HEADERS ${libtiffinclude_HEADERS} ${nodist_libtiffinclude_HEADERS})
set(SOURCES ${HEADERS} ${noinst_HEADERS} ${libtiff_SOURCES})
add_library(tiff SHARED ${SOURCES} ${TIFF_DEF_FILE} ${TIFF_RESOURCES})
add_library(tiff_static STATIC ${SOURCES})
set(TIFF_TARGETS tiff_static tiff)
set(TIFF_LIB tiff)

# Extra libraries support
if (JPEG_SUPPORT)
  include_directories(${JPEG_INCLUDE_DIR})
  target_link_libraries(tiff ${JPEG_LIBRARIES})
  target_link_libraries(tiff_static ${JPEG_LIBRARIES})
endif()
if (ZIP_SUPPORT OR PIXARLOG_SUPPORT)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(tiff ${ZLIB_LIBRARIES})
  target_link_libraries(tiff_static ${ZLIB_LIBRARIES})
endif()
if (JBIG_SUPPORT)
  include_directories(${JBIG_INCLUDE_DIRS})
  target_link_libraries(tiff ${JBIG_LIBRARIES})
  target_link_libraries(tiff_static ${JBIG_LIBRARIES})
endif()

if (NOT MSVC)
  # Other compilers don't conflict with the shared and dynamic libraries
  # having the same base name
  set_target_properties (tiff_static PROPERTIES OUTPUT_NAME tiff)
endif()

# Disable the secure crt warnings in MSVC
if(MSVC)
  set_target_properties(${TIFF_TARGETS}
    PROPERTIES
    COMPILE_FLAGS "/wd4996"
  )
  set_target_properties(tiff PROPERTIES COMPILE_DEFINITIONS TIFF_DLL)
endif()

# Distinguish between release and debug builds with a postfix
set_target_properties(${TIFF_TARGETS}
  PROPERTIES
  DEBUG_POSTFIX "d"
)

# Specify the list of public headers and the version
set_target_properties(${TIFF_TARGETS}
  PROPERTIES
  PUBLIC_HEADER "${HEADERS}"
  VERSION "${LIBTIFF_VERSION}"
  SOVERSION "${LIBTIFF_SOVERSION}"
  DEFINE_SYMBOL "TIFF_INTERNAL"
)

install(TARGETS ${TIFF_TARGETS}
#  EXPORT "tiff"
  RUNTIME DESTINATION "bin"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  PUBLIC_HEADER DESTINATION "include"
)
#install(EXPORT "tiff" DESTINATION "share/cmake")
