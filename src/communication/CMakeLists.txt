file(GLOB COMMUNICATION_SOURCE   *.cpp
                        protocols/*.cpp)
file(GLOB COMMUNICATION_HEADER   *.h
                        protocols/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library (Communication ${COMMUNICATION_SOURCE} ${COMMUNICATION_HEADER})

target_include_directories (Communication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries (Communication LINK_PUBLIC Core)
target_link_libraries (Communication LINK_PUBLIC Renderer)

