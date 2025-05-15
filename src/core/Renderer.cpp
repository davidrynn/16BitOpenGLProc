#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "Debug.h"
#include "Shader.h"
#include "Camera.h"

Renderer::Renderer(Camera &camera)
    : VAO(0), VBO(0), EBO(0), camera(camera), shader(nullptr)
{
}

void Renderer::initialize(std::shared_ptr<Terrain> terrainPtr)
{
    this->terrain = terrainPtr;
    // Create and store shader
    shader = std::make_unique<Shader>("shaders/terrain.vert", "shaders/terrain.frag");
    shader->use(); // Use the shader once at initialization
    shader->setVec3("lightDir", glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    shader->setVec3("baseColor", glm::vec3(0.4f, 0.8f, 0.4f)); // grassy color
    
    skyGradient = std::make_unique<SkyGradient>();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark gray background

    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
    gridRenderer = std::make_unique<GridRenderer>(200, 1.0f);
    armRenderer = std::make_unique<ArmRenderer>();
    armRenderer->initialize();
    // armModel = loadGLTFModel("models/RiggedFigure.glb");
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skyGradient->render(); // ✅ Renders fullscreen background
    shader->use();         // Reuse the shader instead of reloading it

    // Get the current framebuffer size
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // Set up camera and projection matrices
    glm::mat4 view = camera.getViewMatrix();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    // Render terrain
    for (const auto &pair : terrain->getChunks())
    {
        const std::shared_ptr<Chunk> &chunk = pair.second;
        if (chunk && chunk->isUploaded())
        {
            chunk->render(*shader);
        }
    }

    // Render arm
    armRenderer->render(camera, projection);
//     Shader armShader = Shader("shaders/arm.vert", "shaders/arm.frag");
//     armShader.use();
//     armShader.setVec3("baseColor", glm::vec3(0.12f, 0.02f, 0.2f));
//     glm::vec3 forward = glm::normalize(camera.front);
// glm::vec3 position = camera.position + forward * 2.0f;
// armShader.setMat4("view", camera.getViewMatrix());
// armShader.setMat4("projection", projection);
// glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

// armShader.setMat4("model", model);
//     drawModel(armModel);
    // ✅ Reset polygon mode BEFORE drawing grid
    if (wireframeEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset
    }

    // ✅ Draw grid after terrain so it overlays cleanly
    if (wireframeEnabled && gridRenderer)
    {
        glDisable(GL_DEPTH_TEST); // Optional: make grid always visible
        gridRenderer->render(view, projection, glm::vec3(0.3f));
        glEnable(GL_DEPTH_TEST);
    }
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

void Renderer::triggerArmPunch()
{
    armRenderer->triggerPunch(); // <- actually activates animation
}