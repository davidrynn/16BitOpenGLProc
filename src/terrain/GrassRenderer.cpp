#include "GrassRenderer.h"
#include "Shader.h" // Your existing shader class
#include <glad/glad.h>
#include <iostream>

GrassRenderer::GrassRenderer() : VAO(0), VBO(0), instanceVBO(0) {}

GrassRenderer::~GrassRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instanceVBO);
}

void GrassRenderer::initialize() {
    shader = std::make_unique<Shader>("shaders/grass.vert", "shaders/grass.frag");
    setupBlade();
}

void GrassRenderer::setupBlade() {
    float triangle[] = {
        // x, y, z
         0.0f, 0.0f, 0.0f,
         0.05f, 0.2f, 0.0f,
        -0.05f, 0.2f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    // Triangle vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Instance positions
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // Initially empty
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1); // Advance per instance

    glBindVertexArray(0);
}

void GrassRenderer::update(const std::vector<glm::vec3>& newPositions) {
    grassPositions = newPositions;
    updateInstanceBuffer();
}

void GrassRenderer::updateInstanceBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, grassPositions.size() * sizeof(glm::vec3),
                 grassPositions.data(), GL_DYNAMIC_DRAW);
}

void GrassRenderer::render(const glm::mat4& view, const glm::mat4& projection, float time) {
    if (grassPositions.empty()) return;

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setFloat("time", time);

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, static_cast<GLsizei>(grassPositions.size()));
    glBindVertexArray(0);
}