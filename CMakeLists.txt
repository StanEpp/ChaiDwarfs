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
    src/rendering/TextureFactory.cpp
    src/rendering/ShaderManager.cpp
    src/rendering/Texture2D.cpp
    src/rendering/RenderSystem.cpp
    src/interfaces/TerrainObjectInterface.cpp
    src/interfaces/TerrainInterface.cpp
    src/cmpSystems/TerrainObjectSystem.cpp
    src/ecs/Components.cpp
    src/ecs/ComponentsAI.cpp
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
    src/rendering/TextureFactory.hpp
    src/rendering/ScreenquadRenderer.hpp
    src/rendering/RenderSystem.hpp
    src/rendering/OrthographicCamera.hpp
    src/rendering/TileRenderer.hpp
    src/rendering/Texture2DArray.hpp
    src/rendering/Texture2D.hpp
    src/interfaces/TerrainInterface.hpp
    src/interfaces/TerrainObjectInterface.hpp
    src/ecs/EntityManager.hpp
    src/ecs/Components.hpp
    src/ecs/ComponentsAI.hpp
    src/ecs/ECSFactory.hpp
    src/ecs/Entity.hpp
    src/ecs/BaseComponent.hpp
    src/cmpSystems/TerrainObjectSystem.hpp
    src/cmpSystems/ComponentSystem.hpp
    src/cmpSystems/TouchValueSys.cpp
    src/cmpSystems/TouchHealSys.cpp
    src/cmpSystems/DwarfAISys.cpp
    src/cmpSystems/TouchDamageSys.cpp
    src/cmpSystems/TouchDestroySys.cpp
    src/cmpSystems/DamageSys.cpp
    src/cmpSystems/PointSys.cpp
    src/cmpSystems/MoveSys.cpp
    src/cmpSystems/MoveSpriteSys.cpp
    src/cmpSystems/ChangeTerrainTypeSys.cpp
    src/cmpSystems/ChangeTileTypeRenderingSys.cpp
    src/CommandSystem.hpp
    src/ChaiDwarfs.hpp
    src/TerrainMap.hpp
    src/Commands.hpp
    src/ObjectComponentRegistrations.hpp
    src/WindowGLFW.hpp
    src/Timer.hpp
    src/GLFWInput.hpp
    src/PathResolver.hpp.in
)

set(SCRIPT_FILES
    scripts/dwarf.chai
    scripts/mapGeneration.chai
    scripts/objectCreation.chai
    scripts/objectDefinitions.chai
)

set(SHADER_FILES
    shader/tileRendering_fs.glsl
    shader/tileRendering_vs.glsl
)

### Configure the PathResolver header
set(RESOURCE_ROOT_PATH ${CMAKE_SOURCE_DIR})
set(SCRIPT_PATH "scripts")
set(SHADER_PATH "shader")
set(TILE_PATH "tiles")
set(SPRITE_PATH "sprites")
configure_file(src/PathResolver.hpp.in PathResolver.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
###

add_executable(Chaidwarfs
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SCRIPT_FILES}
    ${SHADER_FILES}
)

set_source_files_properties(
    ${SCRIPT_FILES}
    ${SHADER_FILES}
    PROPERTIES
        WRAP_EXCLUDE TRUE
)

add_dependencies(Chaidwarfs glm_ext glfw_ext chaiscript_ext)

target_link_libraries(Chaidwarfs PUBLIC glfw stdc++fs ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
