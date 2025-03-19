cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(16BitCraft_OpenGL)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(src)
add_executable(16BitCraft 
    src/main.cpp 
    src/WindowManager.cpp 
    src/Shader.cpp 
    src/Terrain.cpp)

target_link_libraries(16BitCraft OpenGL::GL glfw GLEW::GLEW)
