add_definitions(-D__APPLE__)
add_definitions(-D__gl_h_)
add_definitions(-Wno-write-strings)
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")