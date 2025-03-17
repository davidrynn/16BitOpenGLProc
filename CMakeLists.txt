cmake_minimum_required(VERSION 3.10)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(16BitCraft_OpenGL)

# Find required OpenGL dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Manually set GLEW paths for if needed for VS Code
include_directories(/opt/homebrew/include) 
link_directories(/opt/homebrew/lib)       


# Add the main.cpp file to the build
add_executable(16BitCraft main.cpp)

# Link against OpenGL, GLFW, and GLEW
target_link_libraries(16BitCraft OpenGL::GL glfw GLEW::GLEW)
