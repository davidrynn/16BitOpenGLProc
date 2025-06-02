#include "DebugMarker.h"
#include <glm/gtc/matrix_transform.hpp>

void DebugMarker::initialize() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, // 0
         0.5f, -0.5f, -0.5f, // 1
         0.5f,  0.5f, -0.5f, // 2
        -0.5f,  0.5f, -0.5f, // 3
        -0.5f, -0.5f,  0.5f, // 4
         0.5f, -0.5f,  0.5f, // 5
         0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };

    GLuint indices[] = {
        // Front face
        0, 1, 1, 2, 2, 3, 3, 0,
        // Back face
        4, 5, 5, 6, 6, 7, 7, 4,
        // Sides
        0, 4, 1, 5, 2, 6, 3, 7
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader = std::make_unique<Shader>("shaders/debug_marker.vert", "shaders/debug_marker.frag");
}

void DebugMarker::setPosition(const glm::vec3& pos) {
    position = pos;
}

void DebugMarker::show() { visible = true; }
void DebugMarker::hide() { visible = false; }
bool DebugMarker::isVisible() const { return visible; }

void DebugMarker::render(const glm::mat4& view, const glm::mat4& projection) {
    if (!visible) return;

    shader->use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
}
