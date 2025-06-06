#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "Shader.h"
#include "Camera.h"
#include "IArmRenderer.h"

class TriArmRenderer : public IArmRenderer {
public:
    void initialize() override;
    void triggerPunch() override;
    void startClench() override { isHolding = true; }  // Start tracking hold
    void stopClench() override { isHolding = false; }  // Stop tracking hold
    void render(const Camera& camera, const glm::mat4& projection) override;
    void updateRotation(float deltaRotation) override { rotationAngle += deltaRotation; }
    void update(float deltaTime) override;  // New update method for hold time tracking

private:
    std::unique_ptr<Shader> armShader;

    unsigned int triVAO = 0, VBO = 0, EBO = 0;
    bool punching = false;
    bool clenching = false;
    bool isHolding = false;  // Track if right mouse is being held
    float holdTime = 0.0f;   // Track how long right mouse has been held
    float punchStartTime = 0.0f;
    float rotationAngle = 0.0f;
    float clenchAmount = 0.0f;  // 0.0 = open, 1.0 = fully clenched
}; 