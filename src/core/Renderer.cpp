#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

Renderer::Renderer(Camera& camera)
    : VAO(0), VBO(0), EBO(0), camera(camera)
{
}

void Renderer::initialize()
{
    const int GRID_SIZE = 50;
    std::vector<float> terrainVertices = Terrain::generateTerrain(GRID_SIZE);
    terrainIndices = Terrain::generateIndices(GRID_SIZE);

    std::cout << "Generated terrain data." << std::endl;

    // Check if OpenGL context is active
    if (glfwGetCurrentContext() == nullptr)
    {
        std::cerr << "ERROR: No OpenGL context active!" << std::endl;
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    std::cout << "VAO, VBO, and EBO generated." << std::endl;

    // Check if VAO is valid
    if (VAO == 0 || VBO == 0 || EBO == 0)
    {
        std::cerr << "ERROR: Failed to generate VAO/VBO/EBO!" << std::endl;
        return;
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(float), terrainVertices.data(), GL_STATIC_DRAW);

    std::cout << "VBO bound and data uploaded." << std::endl;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainIndices.size() * sizeof(unsigned int), terrainIndices.data(), GL_STATIC_DRAW);

    std::cout << "EBO bound and data uploaded." << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    std::cout << "VAO configured." << std::endl;

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

    // Use shader program
    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    shader.use();

    // Set up camera and projection matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

   // std::cout << "Drawing " << terrainIndices.size() << " indices." << std::endl;

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, terrainIndices.size(), GL_UNSIGNED_INT, 0);

    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}
