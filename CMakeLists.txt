cmake_minimum_required(VERSION 3.10)

project(16BitCraft_OpenGL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Include directories
include_directories(
    src
    src/core
    src/entities
    src/terrain
)

# Add executable and all source files
add_executable(16BitCraft 
    src/main.cpp
    src/core/Application.cpp
    src/core/WindowManager.cpp
    src/core/InputManager.cpp
    src/core/Camera.cpp
    src/core/Chunk.cpp
    src/core/Shader.cpp
    src/core/Renderer.cpp
    src/core/SkyGradient.cpp
    src/entities/Player.cpp
    src/entities/Entity.cpp
    src/terrain/Biome.cpp
    src/terrain/BiomeManager.cpp
    src/terrain/MountainsNoise.cpp
    src/terrain/PlainsNoise.cpp
    src/terrain/Terrain.cpp
    src/terrain/TerrainNoiseFactory.cpp
)
# Copy shaders folder to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Link OpenGL, GLFW, and GLEW
target_link_libraries(16BitCraft GLEW::GLEW OpenGL::GL glfw)

# Define GL_SILENCE_DEPRECATION to silence OpenGL deprecation warnings on macOS
add_definitions(-DGL_SILENCE_DEPRECATION)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)