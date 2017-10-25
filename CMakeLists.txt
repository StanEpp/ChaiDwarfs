cmake_minimum_required(VERSION 3.5)

project(Chaidwarfs)

add_subdirectory(deps)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

include_directories(${DEPENDENCY_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES
  src/gl3w.c
  src/SpriteRenderer.cpp
  src/DwarfTerrainObjectInterface.cpp
  src/TerrainObjectSystem.cpp
  src/OrthographicCamera.cpp
  src/GLFWInput.cpp
  src/TerrainMap.cpp
  src/Components.cpp
  src/RenderSystem.cpp
  src/DwarfTerrainInterface.cpp
  src/Texture2DArray.cpp
  src/ScreenquadRenderer.cpp
  src/TileRenderer.cpp
  src/TextureManager.cpp
  src/ShaderManager.cpp
  src/Texture.cpp
  src/main.cpp
)

set(HEADER_FILES
  src/ShaderBaseModel.hpp
  src/DwarfTerrainInterface.hpp
  src/EntityManager.hpp
  src/DwarfAI.hpp
  src/SpriteRenderer.hpp
  src/Components.hpp
  src/CommandSystem.hpp
  src/ChaiDwarfs.hpp
  src/ShaderManager.hpp
  src/ScreenquadRenderer.hpp
  src/RenderSystem.hpp
  src/TerrainObjectSystem.hpp
  src/DwarfTerrainObjectInterface.hpp
  src/DwarfSystem.hpp
  src/TerrainMap.hpp
  src/ECSFactory.hpp
  src/Commands.hpp
  src/stb_image.h
  src/TextureManager.hpp
  src/Entity.hpp
  src/WindowGLFW.hpp
  src/ChaiScriptAI.hpp
  src/ObjectComponentRegistrations.hpp
  src/OrthographicCamera.hpp
  src/TileRenderer.hpp
  src/Timer.hpp
  src/Texture2DArray.hpp
  src/Texture.hpp
  src/GLFWInput.hpp
  src/DwarfRoundActions.hpp
  src/BaseComponent.hpp
  src/ComponentSystem.hpp
)

add_executable(Chaidwarfs ${SOURCE_FILES} ${HEADER_FILES})

add_dependencies(Chaidwarfs glm_ext glfw_ext chaiscript_ext)

target_link_libraries(Chaidwarfs PUBLIC glfw stdc++fs ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})
file(COPY tiles DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shader DESTINATION ${CMAKE_BINARY_DIR})
