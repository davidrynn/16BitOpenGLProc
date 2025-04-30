#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Terrain.h"
#include "Camera.h"
#include "SkyGradient.h"

class Renderer
{
public:
    Renderer(Camera &camera);
    void initialize(std::shared_ptr<Terrain> terrainPtr);
    void render();

private:
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> terrainIndices;
    Camera &camera;
    Shader *shader;                            // Pointer to the shader program
    SkyGradient* skyGradient;
    int screenWidth = 800, screenHeight = 600; // Screen dimensions
    std::shared_ptr<Terrain> terrain;
};

#endif
