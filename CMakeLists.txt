cmake_minimum_required(VERSION 3.0)
project(ComputerGraphicsExps)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

find_package(OpenGL REQUIRED)


set(glew-cmake_BUILD_STATIC true CACHE BOOL "")
set(glew-cmake_BUILD_SHARED false CACHE BOOL "")
set(ONLY_LIBS true)

add_subdirectory(./glew-cmake)
set(GLEW_INCLUDE_DIR ${INCLUDE_DIR})
unset(INCLUDE_DIR)

set(FREEGLUT_BUILD_SHARED_LIBS false CACHE BOOL "")
set(FREEGLUT_BUILD_DEMOS false CACHE BOOL "")

add_subdirectory(./FreeGLUT/freeglut/freeglut)

set(ONLY_LIBS false)

add_subdirectory(./src)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Wno-unused-parameter")
endif()