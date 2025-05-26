#pragma once
#include <glad/glad.h>
#include <memory>
#include "Shader.h"

class ReticleRenderer {
public:
    void initialize();
    void render();
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    std::unique_ptr<Shader> shader;

};
