cmake_minimum_required(VERSION 3.10)
project(16BitOpenGLProc)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use vcpkg toolchain if available
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Set output directories
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})

# Dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

# Add TinyGLTF (header-only library)
add_library(tinygltf INTERFACE)
target_include_directories(tinygltf INTERFACE ${CMAKE_SOURCE_DIR}/external)

# Core source files
set(CORE_SRC
    src/core/Application.cpp
    src/core/Config.cpp
    src/core/ArmRenderer.cpp
    src/core/Camera.cpp
    src/core/Chunk.cpp
    src/core/Debug.cpp
    src/core/DebugMarker.cpp
    src/core/DefaultChunkFactory.cpp
    src/core/GridRenderer.cpp
    src/core/InputManager.cpp
    src/core/LoadingBar.cpp
    src/core/Model.cpp
    src/core/ModelArmRenderer.cpp
    src/core/Raycaster.cpp
    src/core/Renderer.cpp
    src/core/ReticleRenderer.cpp
    src/core/Shader.cpp
    src/core/SkyGradient.cpp
    src/core/Skybox.cpp
    src/core/SlingshotController.cpp
    src/core/TriArmRenderer.cpp
    src/core/WindowManager.cpp
)

# Entity source files
set(ENTITY_SRC
    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/entities/EarthGlob.cpp
)

# Terrain source files
set(TERRAIN_SRC
    src/terrain/Biome.cpp
    src/terrain/BiomeManager.cpp
    src/terrain/ChunkManager.cpp
    src/terrain/ConfigurableNoise.cpp
    src/terrain/FastNoiseLiteWrapper.cpp
    src/terrain/GrassRenderer.cpp
    src/terrain/GrassSpawner.cpp
    src/terrain/Terrain.cpp
    src/terrain/TerrainManipulator.cpp
    src/terrain/TerrainNoiseFactory.cpp
)

# Include directories
include_directories(
    src
    src/core
    src/entities
    src/terrain
    external
    ${GLFW3_INCLUDE_DIRS}
    ${GLAD_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

# Add definitions for TinyGLTF
add_definitions(
    -DTINYGLTF_NO_EXTERNAL_IMAGE   # Use stb_image from your existing setup
    -DTINYGLTF_NO_STB_IMAGE_WRITE  # We don't need image writing capability
)

# Game Core library
add_library(game_core STATIC
    ${CORE_SRC}
    ${ENTITY_SRC}
    ${TERRAIN_SRC}
)
target_link_libraries(game_core PUBLIC 
    OpenGL::GL 
    glfw 
    glad::glad 
    glm::glm
    nlohmann_json::nlohmann_json
    tinygltf
)

# Main executable
add_executable(16BitCraft src/main.cpp)
target_link_libraries(16BitCraft PRIVATE game_core)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Unit Tests
enable_testing()

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(tests
    tests/test_main.cpp
    tests/terrain/TerrainTest.cpp
)
target_link_libraries(tests
    PRIVATE game_core
    PRIVATE glad::glad
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main
)

target_include_directories(tests PRIVATE
    src
    src/core
    src/entities
    src/terrain
    tests
)

gtest_discover_tests(tests)
