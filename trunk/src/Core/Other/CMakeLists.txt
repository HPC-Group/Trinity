file(GLOB BASICS_SOURCE *.c
                        *.cpp
                        Checksums/*.cpp
                        Systeminfo/*.cpp
                        Controller/*.cpp
                        ServiceProtocols/*.cpp)
file(GLOB BASICS_HEADER *.h *.inl
                        Checksums/*.h
                        Systeminfo/*.h
                        Controller/*.h
                        ServiceProtocols/*.h)

add_library (basics ${BASICS_SOURCE} ${BASICS_HEADER})

target_include_directories (basics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries (basics LINK_PUBLIC Core)
target_link_libraries (basics LINK_PUBLIC DataIO)
