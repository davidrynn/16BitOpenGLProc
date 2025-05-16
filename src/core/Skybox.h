#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include "Shader.h"

class Skybox {
public:
    Skybox();
    ~Skybox();
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO, VBO;
    Shader* shader;
    void setupCube();
};

#endif
