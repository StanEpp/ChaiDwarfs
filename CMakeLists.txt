cmake_minimum_required(VERSION 3.5)

project(Chaidwarfs)

add_subdirectory(deps)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

include_directories(${DEPENDENCY_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES
  src/rendering/SpriteRenderer.cpp
  src/rendering/OrthographicCamera.cpp
  src/rendering/Texture2DArray.cpp
  src/rendering/ScreenquadRenderer.cpp
  src/rendering/TileRenderer.cpp
  src/rendering/TextureManager.cpp
  src/rendering/ShaderManager.cpp
  src/rendering/Texture.cpp
  src/rendering/RenderSystem.cpp
  src/interfaces/DwarfTerrainObjectInterface.cpp
  src/interfaces/DwarfTerrainInterface.cpp
  src/cmpSystems/TerrainObjectSystem.cpp
  src/ecs/Components.cpp
  src/GLFWInput.cpp
  src/gl3w.c
  src/TerrainMap.cpp
  src/main.cpp
)

set(HEADER_FILES
  src/rendering/ShaderBaseModel.hpp
  src/rendering/SpriteRenderer.hpp
  src/rendering/ShaderManager.hpp
  src/rendering/stb_image.h
  src/rendering/TextureManager.hpp
  src/rendering/ScreenquadRenderer.hpp
  src/rendering/RenderSystem.hpp
  src/rendering/OrthographicCamera.hpp
  src/rendering/TileRenderer.hpp
  src/rendering/Texture2DArray.hpp
  src/rendering/Texture.hpp
  src/interfaces/DwarfTerrainInterface.hpp
  src/interfaces/DwarfTerrainObjectInterface.hpp
  src/interfaces/DwarfAI.hpp
  src/ecs/EntityManager.hpp
  src/ecs/Components.hpp
  src/ecs/ECSFactory.hpp
  src/ecs/Entity.hpp
  src/ecs/BaseComponent.hpp
  src/cmpSystems/TerrainObjectSystem.hpp
  src/cmpSystems/ComponentSystem.hpp
  src/DwarfSystem.hpp
  src/CommandSystem.hpp
  src/ChaiDwarfs.hpp
  src/TerrainMap.hpp
  src/Commands.hpp
  src/ChaiScriptAI.hpp
  src/ObjectComponentRegistrations.hpp
  src/WindowGLFW.hpp
  src/Timer.hpp
  src/GLFWInput.hpp
  src/DwarfRoundActions.hpp
)

add_executable(Chaidwarfs ${SOURCE_FILES} ${HEADER_FILES})

add_dependencies(Chaidwarfs glm_ext glfw_ext chaiscript_ext)

target_link_libraries(Chaidwarfs PUBLIC glfw stdc++fs ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})
file(COPY tiles DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shader DESTINATION ${CMAKE_BINARY_DIR})
