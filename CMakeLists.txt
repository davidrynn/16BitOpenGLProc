cmake_minimum_required(VERSION 3.10)

project(16BitCraft_OpenGL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------
# Dependencies
# ---------------------------------------

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# ---------------------------------------
# Global Compiler Flags
# ---------------------------------------

add_definitions(-DGL_SILENCE_DEPRECATION)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# ---------------------------------------
# Include Directories
# ---------------------------------------

include_directories(
    src
    src/core
    src/entities
    src/terrain
)

# ---------------------------------------
# Core Library (logic, systems, terrain)
# ---------------------------------------

add_library(game_core STATIC
    src/core/Application.cpp
    src/core/WindowManager.cpp
    src/core/InputManager.cpp
    src/core/Camera.cpp
    src/core/Chunk.cpp
    src/core/DefaultChunkFactory.cpp
    src/core/LoadingBar.cpp
    src/core/Shader.cpp
    src/core/Renderer.cpp
    src/core/SkyGradient.cpp
    src/entities/Player.cpp
    src/entities/Entity.cpp
    src/terrain/Biome.cpp
    src/terrain/BiomeManager.cpp
    src/terrain/MountainsNoise.cpp
    src/terrain/PlainsNoise.cpp
    src/terrain/DesertNoise.cpp
    src/terrain/SnowNoise.cpp
    src/terrain/Terrain.cpp
    src/terrain/TerrainNoiseFactory.cpp
    src/terrain/FastNoiseLiteWrapper.cpp
)

target_link_libraries(game_core
    GLEW::GLEW
    OpenGL::GL
    glfw
)

# ---------------------------------------
# Main Game Executable
# ---------------------------------------

add_executable(16BitCraft
    src/main.cpp
)

target_link_libraries(16BitCraft game_core)

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# ---------------------------------------
# GoogleTest Setup
# ---------------------------------------

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_build_tests OFF CACHE BOOL "Disable gtest's own tests" FORCE)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ---------------------------------------
# Unit Tests
# ---------------------------------------

add_executable(tests
    tests/test_main.cpp
    tests/terrain/TerrainTest.cpp
)

target_link_libraries(tests
    gtest_main
    game_core
)

include(GoogleTest)
gtest_discover_tests(tests)

target_include_directories(tests PRIVATE
    tests
    tests/terrain
)