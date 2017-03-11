#This probably won't build a library for Windows, and it hasn't been tested on OS X

cmake_minimum_required( VERSION 2.6 )

project( GLTools )

find_package(OpenGL REQUIRED)

set ( INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	${INCLUDE_DIRS}
)


include_directories (
	${INCLUDE_DIRS}
)
	
	
set ( GLTOOLS_HDRS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLBatchBase.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLBatch.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLFrame.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLFrustum.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLGeometryTransform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLMatrixStack.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLShaderManager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLTools.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/GLTriangleBatch.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/math3d.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/StopWatch.h"
)

set ( GLTOOLS_SRCS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/GLBatch.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/GLShaderManager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/GLTools.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/GLTriangleBatch.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/math3d.cpp"
)

add_library ( gltools ${GLTOOLS_SRCS})

target_include_directories(
      gltools PUBLIC
      ${FREEGLUT_INCLUDE_DIR}
      ${GLEW_INCLUDE_DIR})

target_link_libraries (gltools ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})