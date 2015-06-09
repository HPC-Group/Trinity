# Experimental CMake build system for libtiff. Tested only on Windows/MSVC.

cmake_minimum_required(VERSION 2.8.3)
enable_testing()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
include("AutoconfHelper")
include("FeatureSummary")
include("CMakePushCheckState")
include("CMakeDependentOption")

project(TIFF)

# Parse the version string from configure.ac
ac_get_version(LibTIFF)

# Basic init
ac_init("LibTIFF Software" ${LIBTIFF_VERSION} "tiff@lists.maptools.org" "tiff")
  
# Extract the individual version components
if ("${LIBTIFF_VERSION}" MATCHES "([0-9]+)\\.([0-9]+)\\.([0-9]+)")
  set(TIFF_VER_MAJOR ${CMAKE_MATCH_1})
  set(TIFF_VER_MINOR ${CMAKE_MATCH_2})
  set(TIFF_VER_REVISION ${CMAKE_MATCH_3})
endif()



#############################################################################
# Configuration tests
#############################################################################

# We don't need to add math library to all targets
if(NOT (WIN32 OR APPLE))
  CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIBM) 
endif()

# Checks for header files.
ac_header_stdc()
ac_check_headers(
  "assert.h"
  "dlfcn.h"
  "fcntl.h"
  "GLUT/glut.h"
  "GL/glut.h"
  "GL/glu.h"
  "GL/gl.h"
  "inttypes.h"
  "io.h"
  "limits.h"
  "malloc.h"
  "memory.h"
  "OpenGL/glu.h"
  "OpenGL/gl.h"
  "search.h"
  "stdint.h"
  "stdlib.h"
  "strings.h"
  "string.h"
  "sys/stat.h"
  "sys/time.h"
  "sys/types.h"
  "unistd.h"
  "windows.h"
)

# Checks for typedefs, structures, and compiler characteristics.
ac_c_const()
ac_c_inline()
ac_c_bigendian()
ac_type_off_t()
ac_type_size_t()
ac_header_time()
ac_struct_tm()

# ---------------------------------------------------------------------------
# Compute sized types for current CPU and compiler options
# ---------------------------------------------------------------------------

# Obtain size of an 'signed short' and define as SIZEOF_SIGNED_SHORT
ac_check_sizeof("signed short")

# Obtain size of an 'unsigned short' and define as SIZEOF_UNSIGNED_SHORT
ac_check_sizeof("unsigned short")

# Obtain size of an 'signed int' and define as SIZEOF_SIGNED_INT
ac_check_sizeof("signed int")

# Obtain size of an 'unsigned int' and define as SIZEOF_UNSIGNED_INT
ac_check_sizeof("unsigned int")

# Obtain size of a 'signed long' and define as SIZEOF_SIGNED_LONG
ac_check_sizeof("signed long")

# Obtain size of a 'unsigned long' and define as SIZEOF_UNSIGNED_LONG
ac_check_sizeof("unsigned long")

# Obtain size of a 'long long' and define as SIZEOF_SIGNED_LONG_LONG.
# If 'long long' is not supported then the value defined is zero.
ac_check_sizeof("signed long long")

# Obtain size of a 'unsigned long long' and define as
# SIZEOF_UNSIGNED_LONG_LONG.  If 'unsigned long long' is not
# supported then the value defined is zero.
ac_check_sizeof("unsigned long long")

# Signed 8-bit type
set(TIFF_INT8_T "signed char")

# Unsigned 8-bit type
set(TIFF_UINT8_T "unsigned char")

# Signed 16-bit type
set(TIFF_INT16_T "signed short")

# Unsigned 16-bit type
set(TIFF_UINT16_T "unsigned short")

# Checking for signed 32-bit type
if(SIZEOF_SIGNED_INT EQUAL 4)
  set(INT32_T      "signed int")
  set(INT32_FORMAT "\"%d\"")
elseif(SIZEOF_SIGNED_LONG EQUAL 4)
  set(INT32_T      "signed long")
  set(INT32_FORMAT "\"%ld\"")
endif()
if (MSVC)
  set(INT32_T "__int32")
endif()
# Signed 32-bit type
set(TIFF_INT32_T "${INT32_T}")
# Signed 32-bit type formatter
set(TIFF_INT32_FORMAT "${INT32_FORMAT}")

# Checking for unsigned 32-bit type
if(SIZEOF_UNSIGNED_INT EQUAL 4)
  set(UINT32_T      "unsigned int")
  set(UINT32_FORMAT "\"%u\"")
elseif(SIZEOF_UNSIGNED_LONG EQUAL 4)
  set(UINT32_T      "unsigned long")
  set(UINT32_FORMAT "\"%lu\"")
endif()
if (MSVC)
  set(UINT32_T "unsigned __int32")
endif()
# Unigned 32-bit type
set(TIFF_UINT32_T "${UINT32_T}")
# Unigned 32-bit type formatter
set(TIFF_UINT32_FORMAT "${UINT32_FORMAT}")

# Checking for signed 64-bit type
if(SIZEOF_SIGNED_LONG EQUAL 8)
  set(INT64_T "signed long")
  set(INT64_FORMAT "\"%ld\"")
elseif(SIZEOF_SIGNED_LONG_LONG EQUAL 8)
  set(INT64_T "signed long long")
  if (WIN32)
    # MinGW32 understands 'long long', but uses printf from WIN32 CRT
    set(INT64_FORMAT="\"%I64d\"")
  else()
    set(INT64_FORMAT="\"%lld\"")
  endif()
endif()
if (MSVC)
  set(INT64_T "__int64")
endif()
message(STATUS "Checking for signed 64-bit type... ${INT64_T}")
# Signed 64-bit type
set(TIFF_INT64_T "${INT64_T}")
# Signed 64-bit type formatter
set(TIFF_INT64_FORMAT "${INT64_FORMAT}")

# Checking for unsigned 64-bit type
if(SIZEOF_UNSIGNED_LONG EQUAL 8)
  set(UINT64_T "unsigned long")
  set(UINT64_FORMAT "\"%lu\"")
elseif(SIZEOF_UNSIGNED_LONG_LONG EQUAL 8)
  set(UINT64_T "unsigned long long")
  if (WIN32)
    # MinGW32 understands 'unsigned long long', but uses printf from WIN32 CRT
    set(UINT64_FORMAT "\"%I64u\"")
  else()
    set(UINT64_FORMAT "\"%llu\"")
  endif()
endif()
if (MSVC)
  set(UINT64_T "unsigned __int64")
endif()
message(STATUS "Checking for unsigned 64-bit type... ${UINT64_T}")
# Unsigned 64-bit type
set(TIFF_UINT64_T "${UINT64_T}")
# Unsigned 64-bit type formatter
set(TIFF_UINT64_FORMAT "${UINT64_FORMAT}")

# Determine TIFF equivalent of ssize_t
set(SIZEOF_UNSIGNED_CHAR_P ${CMAKE_SIZEOF_VOID_P})
if(NOT MSVC)
  if(SIZEOF_SIGNED_LONG EQUAL CMAKE_SIZEOF_VOID_P)
    set(SSIZE_T "signed long")
    set(SSIZE_FORMAT "\"%ld\"")
  elseif(SIZEOF_SIGNED_LONG_LONG EQUAL CMAKE_SIZEOF_VOID_P)
    set(SSIZE_T "signed long long")
    set(SSIZE_FORMAT "\"%lld\"")
  endif()
else()
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SSIZE_T "__int64")
    set(SSIZE_FORMAT "\"%I64d\"")
  else()
    set(SSIZE_T "__int32")
    set(SSIZE_FORMAT "\"%d\"")
  endif()
endif()
message(STATUS "Checking for signed size type... ${SSIZE_T}")
# Signed size type
set(TIFF_SSIZE_T "${SSIZE_T}")
# Signed size type formatter
set(TIFF_SSIZE_FORMAT "${SSIZE_FORMAT}")

# Determine the type to use for the difference between two pointers.
# We will default to the POSIX ptrdiff_t if it is available, but will
# be prepared for the case when it is not.
ac_check_type(ptrdiff_t)
if ("${PTRDIFF_T}" STREQUAL "unknown")
  set(PTRDIFF_T "${SSIZE_T}")
  set(PTRDIFF_FORMAT "${SSIZE_FORMAT}")
else()
  set(PTRDIFF_T "ptrdiff_t")
  set(PTRDIFF_FORMAT "\"%ld\"")
endif()
message(STATUS "Checking for pointer difference type... ${PTRDIFF_T}")
# Pointer difference type
set(TIFF_PTRDIFF_T "${PTRDIFF_T}")
# Pointer difference type formatter
set(TIFF_PTRDIFF_FORMAT "${PTRDIFF_FORMAT}")

# Some compilers (IBM VisualAge) has these types defined, so check it here:
if (HAVE_INTTYPES_H)
  set(CHECK_PRELUDE "#\ninclude <inttypes.h>\n\n")
endif()
ac_check_types("int8;int16;int32" "${CHECK_PRELUDE}")


# Checks for library functions.
ac_check_funcs("floor" "isascii" "memmove" "memset" "mmap" "pow" "setmode"
  "sqrt" "strchr" "strrchr" "strstr" "strtol" "strtoull")
  
# Will use local replacements for unavailable functions
ac_check_funcs("getopt" "strcasecmp" "strtoul" "strtoull" "lfind")
if (NOT HAVE_GETOPT OR NOT HAVE_STRCASECMP OR NOT HAVE_STRTOUL OR
    NOT HAVE_STRTOULL OR NOT HAVE_LFIND)
  set(NEED_LIBPORT TRUE)
else()
  set(NEED_LIBPORT FALSE)
endif()

# ---------------------------------------------------------------------------
# Check the native cpu bit order [use result from ac_c_bigendian()]
# ---------------------------------------------------------------------------
if (WORDS_BIGENDIAN)
  set(HOST_FILLORDER FILLORDER_MSB2LSB)
  message(STATUS "Checking native cpu bit order... msb2lsb")
else()
  set(HOST_FILLORDER FILLORDER_LSB2MSB)
  message(STATUS "Checking native cpu bit order... lsb2msb")
endif()

# ---------------------------------------------------------------------------
# Configure legacy tifconf.h HOST_BIGENDIAN.
# ---------------------------------------------------------------------------
if (WORDS_BIGENDIAN)
  set(HOST_BIGENDIAN 1)
else()
  set(HOST_BIGENDIAN 0)
endif()

# ---------------------------------------------------------------------------
# Set the floating point format.
# FIXME: write appropriate test.
# ---------------------------------------------------------------------------
set(HAVE_IEEEFP 1)

# ---------------------------------------------------------------------------
# Support large files.
# ---------------------------------------------------------------------------
ac_sys_largefile()

# ---------------------------------------------------------------------------
# Point to path where we should install documentation (relative to prefix)
# ---------------------------------------------------------------------------
set(LIBTIFF_DOCDIR "share/doc/${PACKAGE_UNQUOTED}-${LIBTIFF_VERSION}"
  CACHE STRING "Directory where documentation should be installed.")
  
  
  
#############################################################################
# Switch on/off internal codecs.
#############################################################################

# Helper macro to add features
function(tiff_feature name description default)
  if(default)
    set(default ON)
  else()
    set(default OFF)
  endif()
  string(REPLACE " " "_" name "${name}")
  string(REPLACE "-" "_" name "${name}")
  string(TOUPPER "HAVE_${name}"    have_var)
  string(TOUPPER "${name}_SUPPORT" support_var)
  option(${have_var} "${description}" ${default})
  add_feature_info(${name} ${have_var} "${description}")
  if(${have_var})
    set(${support_var} 1 PARENT_SCOPE)
  endif()
endfunction()

tiff_feature(ccitt "Support CCITT Group 3 & 4 algorithms" ON)
tiff_feature(packbits "Support Macintosh PackBits algorithm." ON)
tiff_feature(lzw "Support LZW algorithm." ON)
tiff_feature(thunder "Support ThunderScan 4-bit RLE algorithm." ON)
tiff_feature(next "Support NeXT 2-bit RLE algorithm." ON)
tiff_feature(logluv "Support LogLuv high dynamic range encoding." ON)

# ---------------------------------------------------------------------------
# Switch on/off support for Microsoft Document Imaging
# ---------------------------------------------------------------------------
tiff_feature(mdi "Support Microsoft Document Imaging format." ON)

# ---------------------------------------------------------------------------
# Check for ZLIB.
# ---------------------------------------------------------------------------
find_package(ZLIB 1.2)

if (ZLIB_FOUND)
  tiff_feature(zip "Support Deflate compression." ON)
endif()

# ---------------------------------------------------------------------------
# Check for Pixar log-format algorithm.
# ---------------------------------------------------------------------------
tiff_feature(pixarlog "Support Pixar log-format algorithm (requires Zlib)." ${ZLIB_FOUND})

# ---------------------------------------------------------------------------
# Check for JPEG.
# ---------------------------------------------------------------------------
find_package(JPEG 6)

if (JPEG_FOUND)
  tiff_feature(jpeg "Support JPEG compression (requires IJG JPEG library)." ON)
endif()

# ---------------------------------------------------------------------------
# Check for Old JPEG.
# ---------------------------------------------------------------------------
tiff_feature(ojpeg "Support Old JPEG compresson (read-only)." ${JPEG_FOUND})

# ---------------------------------------------------------------------------
# Check for JBIG-KIT.
# ---------------------------------------------------------------------------
find_package(JBIG)

if (JBIG_FOUND)
  tiff_feature(jbig "Support ISO JBIG compression." ON)
  
  cmake_push_check_state()
  set(CMAKE_REQUIRED_INCLUDES ${JBIG_INCLUDE_DIRS})
  set(CMAKE_REQUIRED_LIBRARIES ${JBIG_LIBRARIES})
  ac_check_funcs(jbg_newlen)
  cmake_pop_check_state()
endif()

# ---------------------------------------------------------------------------
# Check for liblzma2.
# ---------------------------------------------------------------------------
message("TODO: Find and configure liblzma2")
tiff_feature(lzma "Support LZMA2 compression (configure.ac:711)" OFF)

# ---------------------------------------------------------------------------
# Should 8/12 bit jpeg mode be enabled?
# ---------------------------------------------------------------------------
message("TODO: Find and configure the special flavor of jpeg with 12-bit support (configure.ac:763)")
tiff_feature(jpeg12 "8/12 bit libjpeg dual mode enabled" OFF)
if (HAVE_JPEG12)
  set(JPEG_DUAL_MODE_8_12 1)
endif()

# ---------------------------------------------------------------------------
# Check for C++.
# ---------------------------------------------------------------------------
tiff_feature(cxx "Support C++ stream API (requires C++ compiler)." ON)

# ---------------------------------------------------------------------------
# Check for OpenGL and GLUT.
# ---------------------------------------------------------------------------
find_package(OpenGL)
find_package(GLUT)

if(OPENGL_FOUND AND GLUT_FOUND)
  set(HAVE_OPENGL 1)
endif()

# ---------------------------------------------------------------------------
# Check for Win32 IO: make sure we have windows.h but not cygwin
# ---------------------------------------------------------------------------
if (WIN32 AND NOT CYGWIN)
  set(WIN32_IO 1)
endif()

# ---------------------------------------------------------------------------
# Check for X Athena Widgets
# ---------------------------------------------------------------------------
if (NOT WIN32)
  message("TODO: Find X Athena Widgets (configure.ac:849)")
  set(HAVE_XAW 0)
endif()

# ===========================================================================
# ``Orthogonal Features''
# ===========================================================================

# ---------------------------------------------------------------------------
# Default handling of strip chopping support.
# ---------------------------------------------------------------------------
tiff_feature(stripchop
"Support strip chopping (whether or not to convert single-strip uncompressed
images to mutiple strips of specified size to reduce memory usage)." ON)
if (HAVE_STRIPCHOP)
  set(STRIPCHOP_DEFAULT "TIFF_STRIPCHOP")
  set(STRIP_SIZE_DEFAULT 8192 CACHE STRING
    "Default size of the strip in bytes (when strip chopping enabled).")
  if (NOT STRIP_SIZE_DEFAULT GREATER 0)
    message(FATAL_ERROR "Invalid default strip size: ${STRIP_SIZE_DEFAULT}")
  endif()
endif()

# ---------------------------------------------------------------------------
# Should we try to defer loading of strip/tile offsets and sizes to 
# optimize directory scanning?  These is an experimental feature for 
# libtiff 4.0.
# ---------------------------------------------------------------------------
tiff_feature(defer-strile-load
  "Enable deferred strip/tile offset/size loading (experimental)." OFF)
if (HAVE_DEFER_STRILE_LOAD)
  set(DEFER_STRILE_LOAD 1)
endif()

# ---------------------------------------------------------------------------
# Check for support of CHUNKY_STRIP_READ_SUPPORT, a mechanism to allowing
# reading large strips (usually one strip files) in chunks when using
# TIFFReadScanline().  This is an experimental feature in libtiff 4.0.
# ---------------------------------------------------------------------------
tiff_feature(chunky-strip-read
  "Enable partial strip reading for large strips (experimental)." OFF)
if (HAVE_CHUNKY_STRIP_READ)
  set(CHUNKY_STRIP_READ_SUPPORT 1)
endif()

# ---------------------------------------------------------------------------
# Default subifd support.
# ---------------------------------------------------------------------------
set(SUBIFD_SUPPORT 1)

# ---------------------------------------------------------------------------
# Default handling of ASSOCALPHA support.
# ---------------------------------------------------------------------------
tiff_feature(extrasample-as-alpha
"Treat extra sample as alpha (default enabled). The RGBA interface will treat
a fourth sample with no EXTRASAMPLE_ value as being ASSOCALPHA. Many packages
produce RGBA files but don't mark the alpha properly." ON)
if (HAVE_EXTRASAMPLE_AS_ALPHA)
  set(DEFAULT_EXTRASAMPLE_AS_ALPHA 1)
endif()

# ---------------------------------------------------------------------------
# Default handling of YCbCr subsampling support.
# See Bug 168 in Bugzilla, and JPEGFixupTestSubsampling() for details.
# ---------------------------------------------------------------------------
tiff_feature(check-ycbcr-subsampling
"Pick up YCbCr subsampling info from the JPEG data stream to support files
lacking the tag (default enabled)." ON)
if (HAVE_CHECK_YCBCR_SUBSAMPLING)
  set(CHECK_JPEG_YCBCR_SUBSAMPLING 1)
endif()

###############################################################################

# Create the configuration headers
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/libtiff/tiffconf.h.cmakein"
  "${CMAKE_CURRENT_BINARY_DIR}/headers/tiffconf.h"
)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/libtiff/tif_config.h.cmakein"
  "${CMAKE_CURRENT_BINARY_DIR}/headers/tif_config.h"
)
set(TIFF_CONFIG_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/headers/tiffconf.h"
  "${CMAKE_CURRENT_BINARY_DIR}/headers/tif_config.h"
)
set(TIFF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/headers")


# Main TIFF library
add_subdirectory(libtiff)

option(BUILD_TOOLS "Build the TIFF tools (required for tests)." ON)
if (BUILD_TOOLS)
  # Port helper library (always built for the tools)
  add_subdirectory(port)
  # Tools, required for testing
  add_subdirectory(tools)
endif()

# Test binaries and scripts
CMAKE_DEPENDENT_OPTION(BUILD_TESTS "Build the tests." ON
  "BUILD_TOOLS" OFF)
if (BUILD_TESTS)
  add_subdirectory(test)
endif()

feature_summary(WHAT ALL)
