#include "GridRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Shader.h"  // assumes you have a Shader class

GridRenderer::GridRenderer(int size, float spacing) : visible(true) {
    generateGrid(size, spacing);
    shader = new Shader("shaders/grid_vertex.glsl", "shaders/grid_fragment.glsl");
}

GridRenderer::~GridRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shader;
}

void GridRenderer::generateGrid(int size, float spacing) {
    std::vector<float> vertices;
    for (int i = -size; i <= size; ++i) {
        // lines along X
        vertices.push_back(-size * spacing); vertices.push_back(0.0f); vertices.push_back(i * spacing);
        vertices.push_back(size * spacing);  vertices.push_back(0.0f); vertices.push_back(i * spacing);
        // lines along Z
        vertices.push_back(i * spacing); vertices.push_back(0.0f); vertices.push_back(-size * spacing);
        vertices.push_back(i * spacing); vertices.push_back(0.0f); vertices.push_back(size * spacing);
    }
    vertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void GridRenderer::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& color) {
    if (!visible) return;
    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", glm::mat4(1.0f));
    shader->setVec3("gridColor", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}

void GridRenderer::setVisible(bool v) { visible = v; }
bool GridRenderer::isVisible() const { return visible; }
