#include "ReticleRenderer.h"
#include "Shader.h"

void ReticleRenderer::initialize() {
    // Tiny square for now (centered at origin in NDC)
    float vertices[] = {
        -0.01f,  0.01f,
         0.01f,  0.01f,
         0.01f, -0.01f,
        -0.01f, -0.01f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Load a minimal reticle shader
    shader = std::make_unique<Shader>("shaders/reticle.vert", "shaders/reticle.frag");
    shader->use();

}

void ReticleRenderer::render() {
    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}
