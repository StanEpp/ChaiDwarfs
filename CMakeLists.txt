cmake_minimum_required(VERSION 3.5)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

project(Chaidwarfs)

set(DEPENDENCY_DIR ${CMAKE_BINARY_DIR}/deps)
set(DEPENDENCY_INSTALL_DIR ${CMAKE_BINARY_DIR}/deps/installed)

externalproject_add(glm
  PREFIX ${DEPENDENCY_DIR}/glm
  INSTALL_DIR ${DEPENDENCY_DIR}/glm
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <SOURCE_DIR>/glm ${DEPENDENCY_INSTALL_DIR}/glm
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  GIT_REPOSITORY https://github.com/g-truc/glm.git
)

externalproject_add(glfw
  PREFIX ${DEPENDENCY_DIR}/glfw
  CMAKE_ARGS -DGLFW_BUILD_TESTS=OFF
             -DGLFW_BUILD_EXAMPLES=OFF
             -DGLFW_BUILD_DOCS=OFF
             -DGLFW_INSTALL=OFF
             -DGLFW_BUILD_SHARED_LIBS=ON
  GIT_REPOSITORY https://github.com/glfw/glfw.git
)

set(GLFW_INCLUDE_DIR ${DEPENDENCY_INSTALL_DIR}/glfw/include)
set(GLFW_LIB_DIR ${DEPENDENCY_INSTALL_DIR}/glfw/libglfw3.a)

find_package(Threads REQUIRED)
find_path(CHAISCRIPT_INCLUDE_DIR chaiscript/chaiscript.hpp)

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_FILES "*.hpp" "*.p")

add_executable(Chaidwarfs ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Chaidwarfs PUBLIC ${GLFW_LIB_DIR} stdc++fs ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_dependencies(Chaidwarfs glfw)

target_compile_features(Chaidwarfs PUBLIC cxx_std_17)

file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})
file(COPY tiles DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shader DESTINATION ${CMAKE_BINARY_DIR})
