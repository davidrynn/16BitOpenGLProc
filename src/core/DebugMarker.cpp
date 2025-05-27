#include "DebugMarker.h"
#include <glm/gtc/matrix_transform.hpp>

void DebugMarker::initialize() {
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };

    GLuint cubeIndices[] = {
        0,1,2,3,0,4,5,6,7,4,5,1,2,6,7,3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader = std::make_unique<Shader>("shaders/debug_marker.vert", "shaders/debug_marker.frag");
}

void DebugMarker::setPosition(const glm::vec3& pos) {
    position = pos;
}

void DebugMarker::render(const glm::mat4& view, const glm::mat4& projection) {
    shader->use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 16); // wireframe cube edges
}
