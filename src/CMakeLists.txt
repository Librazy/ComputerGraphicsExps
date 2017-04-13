macro (add_exp _name )
  unset(SRC_LIST)
  aux_source_directory(./${_name} SRC_LIST)
  add_executable(${PROJECT_NAME}_${_name} ${SRC_LIST})
  target_link_libraries(
      ${PROJECT_NAME}_${_name} 
      ${OPENGL_gl_LIBRARY}
      ${OPENGL_glu_LIBRARY}
      ${GLEW_LIBRARY}
      ${GLUT_LIBRARIES}
      ${GLTOOLS_LIBRARY}  )
  
  target_include_directories(
      ${PROJECT_NAME}_${_name} PUBLIC
      ${FREEGLUT_INCLUDE_DIR}
      ${GLEW_INCLUDE_DIR}
      ${GLTOOLS_INCLUDE_DIR} )
  
  if(WIN32)
     set_target_properties(${PROJECT_NAME}_${_name}  PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
     set_target_properties(${PROJECT_NAME}_${_name}  PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
     set_target_properties(${PROJECT_NAME}_${_name}  PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
     set_target_properties(${PROJECT_NAME}_${_name}  PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
  endif(WIN32)

endmacro()

add_exp(exp1)
add_exp(exp2)
add_exp(exp3)
add_exp(exp4)
add_exp(exp5)

unset(FREEGLUT_BUILD_DEMOS CACHE)
unset(FREEGLUT_BUILD_SHARED_LIBS CACHE)
unset(FREEGLUT_BUILD_STATIC_LIBS CACHE)
unset(FREEGLUT_GLES CACHE)
unset(FREEGLUT_PRINT_ERRORS CACHE)
unset(FREEGLUT_PRINT_WARNINGS CACHE)
unset(FREEGLUT_REPLACE_GLUT CACHE)
unset(FREEGLUT_WAYLAND CACHE)
unset(glew-cmake_BUILD_MULTI_CONTEXT CACHE)
unset(glew-cmake_BUILD_SHARED CACHE)
unset(glew-cmake_BUILD_SINGLE_CONTEXT CACHE)
unset(glew-cmake_BUILD_STATIC CACHE)
unset(ONLY_LIBS CACHE) 
unset(INSTALL_PDB CACHE) 
