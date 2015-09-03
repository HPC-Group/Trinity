if (NOT DEFINED NEED_LIBPORT)
  message(FATAL_ERROR "libport is not properly set up")
endif()

if (NEED_LIBPORT)
  add_definitions(-DNEED_LIBPORT)
  set(SRCS getopt.c strcasecmp.c)
else()
  set(SRCS dummy.c)
endif()

add_library(port STATIC ${SRCS})
