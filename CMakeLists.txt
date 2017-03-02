cmake_minimum_required(VERSION 3.0)
project(ComputerGraphicsExps)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

find_package(OpenGL REQUIRED)

set(glew-cmake_BUILD_STATIC true CACHE BOOL "")
set(glew-cmake_BUILD_SHARED false CACHE BOOL "")
set(ONLY_LIBS true CACHE BOOL "")

add_subdirectory(${CMAKE_SOURCE_DIR}/glew-cmake)

set(FREEGLUT_BUILD_SHARED_LIBS false CACHE BOOL "")
set(FREEGLUT_BUILD_DEMOS false CACHE BOOL "")

add_subdirectory(${CMAKE_SOURCE_DIR}/FreeGLUT/freeglut/freeglut)

set(GLUT_LIBRARIES freeglut_static)
set(GLEW_LIBRARY libglew_static)
set(GLEWMX_LIBRARY  libglewmx_static)
set(FREEGLUT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/FreeGLUT/freeglut/freeglut/include)
set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/glew-cmake/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/GLTools)

set(GLTOOLS_LIBRARY gltools)
set(GLTOOLS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/GLTools/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Wno-unused-parameter")
endif()