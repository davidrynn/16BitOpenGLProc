#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Terrain.h"
#include "Camera.h"

class Renderer
{
public:
    Renderer(Camera &camera);
    void initialize();
    void render();

private:
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> terrainIndices;
    Camera &camera;
    Shader *shader;                            // Pointer to the shader program
    int screenWidth = 800, screenHeight = 600; // Screen dimensions
};

#endif
