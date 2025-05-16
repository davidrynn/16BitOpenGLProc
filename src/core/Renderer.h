#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ArmRenderer.h"
#include "Camera.h"
#include "GridRenderer.h"
#include "Shader.h"
#include "SkyGradient.h"
#include "Terrain.h"

class Renderer
{
public:
    Renderer(Camera &camera);
    void initialize(std::shared_ptr<Terrain> terrainPtr);
    void render();
    void triggerArmPunch();
private:
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> terrainIndices;

    Camera& camera;

    std::unique_ptr<GridRenderer> gridRenderer;
    std::unique_ptr<ArmRenderer> armRenderer;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<SkyGradient> skyGradient;

    std::shared_ptr<Terrain> terrain;

    int screenWidth = 800, screenHeight = 600;
};

#endif
