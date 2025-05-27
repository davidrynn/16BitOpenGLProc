#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "Shader.h"

class DebugMarker {
public:
    void initialize();
    void setPosition(const glm::vec3& pos);
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    glm::vec3 position{0.0f};
    std::unique_ptr<Shader> shader;
};
