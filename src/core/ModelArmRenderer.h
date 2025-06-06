#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "IArmRenderer.h"
#include "Model.h"

class ModelArmRenderer : public IArmRenderer {
public:
    void initialize() override;
    void triggerPunch() override;
    void startClench() override { isHolding = true; }
    void stopClench() override { isHolding = false; }
    void render(const Camera& camera, const glm::mat4& projection) override;
    void updateRotation(float deltaRotation) override { rotationAngle += deltaRotation; }
    void update(float deltaTime) override;

    // Debug functions
    void setModelScale(float scale) { modelScale = scale; }
    void setModelPosition(const glm::vec3& pos) { modelPosition = pos; }
    void toggleDebugMode() { debugMode = !debugMode; }

private:
    std::unique_ptr<Shader> armShader;
    std::unique_ptr<Model> armModel;

    bool punching = false;
    bool clenching = false;
    bool isHolding = false;
    bool debugMode = false;  // Toggle for debug visualization
    float holdTime = 0.0f;
    float punchStartTime = 0.0f;
    float clenchAmount = 0.0f;
    float rotationAngle = 0.0f;

    // Debug variables for easy adjustment
    float modelScale = 1.0f;  // Scale factor for the model
    glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, -0.5f);  // Position offset

    // Bone animation
    static const int MAX_BONES = 100;
    std::vector<glm::mat4> boneTransforms;
    void updateBoneTransforms();
}; 