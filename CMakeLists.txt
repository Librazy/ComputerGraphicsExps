cmake_minimum_required(VERSION 3.0)
project(ComputerGraphicsExps)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

if(MSVC)
  if(NOT DEFINED GLUT_ROOT_PATH)
    set(GLUT_ROOT_PATH "./freeglut" CACHE PATH "Path to GLUT library.")
  endif()
endif()

find_package(FreeGLUT REQUIRED)
find_package(OpenGL REQUIRED)

if(NOT GLUT_FOUND)
  if((NOT DEFINED GLUT_ROOT_PATH) OR (GLUT_ROOT_PATH EQUAL "GLUT_ROOT_PATH-NOTFOUND") OR (GLUT_ROOT_PATH EQUAL "./freeglut"))
    set(GLUT_ROOT_PATH "GLUT_ROOT_PATH-NOTFOUND" CACHE PATH "Path to GLUT library.")
    message(FATAL_ERROR "Path to GLUT library not set.")
  endif()
endif()

if(MSVC)
  link_directories(
    ${GLUT_ROOT_PATH}/lib
  )
endif()

aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(
    ${PROJECT_NAME} 
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY} )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${GLUT_INCLUDE_DIR} )

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
	FREEGLUT_STATIC
)

if(NOT MSVC)
  target_link_libraries(
    ${PROJECT_NAME} 
    ${GLUT_LIBRARIES} )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Wno-unused-parameter")
endif()