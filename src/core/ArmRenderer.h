#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Shader.h"
#include "Camera.h"

class ArmRenderer {
public:
    void initialize();
    void triggerPunch();
    void render(const Camera& camera, const glm::mat4& projection);

private:
    std::unique_ptr<Shader> armShader;

    unsigned int cubeVAO = 0, VBO = 0;
    bool punching = false;
    float punchStartTime = 0.0f;
};
