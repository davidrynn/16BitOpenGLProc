#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;
class Camera;
class Player;

class SlingshotController {
public:
    void update(GLFWwindow* window, Camera& camera, Player& player);

private:
    bool isCharging = false;
    bool wasChargingLastFrame = false;
    double initialMouseY = 0.0;
    float chargeAmount = 0.0f;
    glm::vec3 lockedDirection = glm::vec3(0.0f);
    bool chargingViewLocked = false;
    
    const float chargeMultiplier = 0.3f;
    const float maxCharge = 100.0f;
    const float powerFactor = 30.0f;
};