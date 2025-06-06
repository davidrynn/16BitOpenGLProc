#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.h"

class IArmRenderer {
public:
    virtual ~IArmRenderer() = default;
    virtual void initialize() = 0;
    virtual void triggerPunch() = 0;
    virtual void startClench() = 0;
    virtual void stopClench() = 0;
    virtual void render(const Camera& camera, const glm::mat4& projection) = 0;
    virtual void updateRotation(float deltaRotation) = 0;
    virtual void update(float deltaTime) = 0;
}; 