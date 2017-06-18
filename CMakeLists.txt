cmake_minimum_required(VERSION 3.5)

project(Chaidwarfs)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)
find_path(CHAISCRIPT_INCLUDE_DIR chaiscript/chaiscript.hpp)

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_FILES "*.hpp" "*.p")

add_executable(Chaidwarfs ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Chaidwarfs PUBLIC glfw stdc++fs ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

target_compile_features(Chaidwarfs PUBLIC cxx_std_17)

file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})
file(COPY tiles DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shader DESTINATION ${CMAKE_BINARY_DIR})
