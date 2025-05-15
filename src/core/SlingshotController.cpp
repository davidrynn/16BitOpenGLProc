#include "SlingshotController.h"
#include <algorithm> 
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "InputManager.h"
#include "Camera.h"
#include "Player.h"

void SlingshotController::update(GLFWwindow* window, Camera& camera, Player& player) {
    bool leftDown  = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)  == GLFW_PRESS;
    bool rightDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    bool bothDown = leftDown && rightDown;

    if (bothDown && !isCharging) {
        isCharging = true;
        chargeAmount = 0.0f;
        double mouseX;
        glfwGetCursorPos(window, &mouseX, &initialMouseY);
        lockedDirection = glm::normalize(camera.front);
        chargingViewLocked = true;
        InputManager::freezeCamera();
        std::cout << "Started charging" << std::endl;
    }

    if (isCharging && bothDown) {
        double mouseX, currentMouseY;
        glfwGetCursorPos(window, &mouseX, &currentMouseY);
        double deltaY = currentMouseY - initialMouseY;
        chargeAmount = std::clamp(static_cast<float>(deltaY) * chargeMultiplier, 0.0f, maxCharge);
        std::cout << "Charging: " << chargeAmount << std::endl;
    }

    if (wasChargingLastFrame && isCharging && !bothDown) {
        glm::vec3 force = lockedDirection * chargeAmount * powerFactor;
        player.applyForce(force);
        std::cout << "Launched with force: (" 
                  << force.x << ", " << force.y << ", " << force.z << ")" << std::endl;
        isCharging = false;
        chargeAmount = 0.0f;
        lockedDirection = glm::vec3(0.0f);
        InputManager::unfreezeCamera();
    }

    wasChargingLastFrame = isCharging;
}
