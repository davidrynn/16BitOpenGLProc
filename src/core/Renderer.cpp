#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

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
    shader = new Shader("shaders/terrain_vertex.glsl", "shaders/terrain_fragment.glsl");
    shader->use(); // Use the shader once at initialization
    shader->setVec3("lightDir", glm::normalize(glm::vec3(1.0f, -1.0f, 0.5f))); // Add this!

    shader->setVec3("baseColor", glm::vec3(0.4f, 0.8f, 0.4f)); // grassy color
    skyGradient = new SkyGradient();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark gray background

    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skyGradient->render(); // ✅ Renders fullscreen background
    shader->use(); // Reuse the shader instead of reloading it

    // Get the current framebuffer size
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // Set up camera and projection matrices
    glm::mat4 view = camera.getViewMatrix();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    // Render terrain
    terrain->render(*shader, camera.position.x, camera.position.z);

    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}