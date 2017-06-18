cmake_minimum_required(VERSION 3.5)
project(Chaidwarfs)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_path(CHAISCRIPT_INCLUDE_DIR chaiscript/chaiscript.hpp)
add_executable(Chaidwarfs 
  src/BaseComponent.hpp
  src/ChaiDwarfs.hpp
  src/Commands.hpp
  src/CommandSystem.hpp
  src/Components.hpp
  src/Components.cpp
  src/ComponentSystem.hpp
  src/DwarfAI.hpp
  src/DwarfRoundActions.hpp
  src/DwarfSystem.hpp
  src/DwarfTerrainInterface.hpp
  src/DwarfTerrainInterface.cpp
  src/DwarfTerrainObjectInterface.hpp
  src/DwarfTerrainObjectInterface.cpp
  src/ECSFactory.hpp
  src/Entity.hpp
  src/EntityManager.hpp
  src/gl3w.c
  src/GLFWInput.cpp
  src/GLFWInput.hpp
  src/main.cpp
  src/ObjectComponentRegistrations.hpp
  src/OrthographicCamera.cpp
  src/OrthographicCamera.hpp
  src/RenderSystem.hpp
  src/RenderSystem.cpp
  src/ScreenquadRenderer.cpp
  src/ScreenquadRenderer.hpp
  src/ShaderBaseModel.hpp
  src/ShaderManager.cpp
  src/ShaderManager.hpp
  src/SpriteRenderer.cpp
  src/SpriteRenderer.hpp
  src/stb_image.h
  src/TerrainMap.hpp
  src/TerrainMap.cpp
  src/TerrainObjectSystem.hpp
  src/TerrainObjectSystem.cpp
  src/Texture.cpp
  src/Texture.hpp
  src/Texture2DArray.cpp
  src/Texture2DArray.hpp
  src/TextureManager.hpp
  src/TextureManager.cpp
  src/TileRenderer.cpp
  src/TileRenderer.hpp
  src/Timer.hpp
  src/WindowGLFW.hpp
)
target_compile_features(Chaidwarfs PUBLIC cxx_std_17)
