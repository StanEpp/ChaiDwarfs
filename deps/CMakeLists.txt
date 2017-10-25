include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(DEPENDENCY_DIR ${CMAKE_BINARY_DIR}/deps)
set(DEPENDENCY_INSTALL_DIR ${DEPENDENCY_DIR}/installed)
set(DEPENDENCY_INSTALL_INCLUDE_DIR ${DEPENDENCY_INSTALL_DIR}/include)
set(DEPENDENCY_INSTALL_LIB_DIR ${DEPENDENCY_INSTALL_DIR}/lib)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${DEPENDENCY_INSTALL_LIB_DIR})
message(STATUS ${CMAKE_PREFIX_PATH})
set(DEPENDENCY_INCLUDE_DIR ${CMAKE_BINARY_DIR}/deps/installed/include PARENT_SCOPE)

externalproject_add(glm_ext
  PREFIX ${DEPENDENCY_DIR}/glm
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <SOURCE_DIR>/glm ${DEPENDENCY_INSTALL_INCLUDE_DIR}/glm
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.8.5
)

externalproject_add(chaiscript_ext
  PREFIX ${DEPENDENCY_DIR}/chaiscript
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <SOURCE_DIR>/include ${DEPENDENCY_INSTALL_INCLUDE_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  GIT_REPOSITORY https://github.com/ChaiScript/ChaiScript.git
  GIT_TAG c++17
)

externalproject_add(gl3w_ext
  PREFIX ${DEPENDENCY_DIR}/gl3w
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <BINARY_DIR>/include ${DEPENDENCY_INSTALL_INCLUDE_DIR}
                  ${CMAKE_COMMAND} -E copy_directory
                  <BINARY_DIR>/src ${DEPENDENCY_INSTALL_DIR}/src/GL
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  GIT_REPOSITORY https://github.com/skaslev/gl3w.git
)

externalproject_add(glfw_ext
  PREFIX ${DEPENDENCY_DIR}/glfw
  CMAKE_ARGS -DGLFW_BUILD_TESTS=OFF
            -DGLFW_BUILD_EXAMPLES=OFF
            -DGLFW_BUILD_DOCS=OFF
            -DGLFW_INSTALL=ON
            -DBUILD_SHARED_LIBS=ON
            -DCMAKE_INSTALL_PREFIX=${DEPENDENCY_INSTALL_DIR}
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.2.1
)

add_library(glfw SHARED IMPORTED GLOBAL)
find_library(GLFW_LIB_DIR glfw)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GLFW_LIB_DIR})

set(GL3W_SRC_FILE ${CMAKE_BINARY_DIR}/deps/installed/src/GL/gl3w.c PARENT_SCOPE)