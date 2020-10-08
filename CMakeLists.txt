cmake_minimum_required(VERSION 3.10)

project(bot CXX)

set(CPP_COMPILE_FLAG "-g -Wall -DENABLE_LOG")

include_directories(${CMAKE_SOURCE_DIR}/shared_lib
                    ${CMAKE_SOURCE_DIR}/bot
                    ${CMAKE_SOURCE_DIR}/dependencies)

file(GLOB_RECURSE SHARED_LIB_SOURCES
     RELATIVE ${CMAKE_SOURCE_DIR} "shared_lib/*.cpp")
file(GLOB_RECURSE BOT_SOURCES
     RELATIVE ${CMAKE_SOURCE_DIR} "bot/*.cpp")

add_definitions(${CPP_COMPILE_FLAG})

add_library(bot_shared_lib STATIC ${SHARED_LIB_SOURCES})
set_property(TARGET bot_shared_lib PROPERTY CXX_STANDARD 11)

add_executable(bot ${BOT_SOURCES})
set_property(TARGET bot PROPERTY CXX_STANDARD 11)
target_link_libraries(bot bot_shared_lib GL GLEW glfw3 dl m Xrandr
                      Xi X11 Xxf86vm Xinerama Xcursor pthread)

