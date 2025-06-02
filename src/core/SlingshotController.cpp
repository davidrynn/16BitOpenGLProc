#include "SlingshotController.h"
#include <algorithm> 
#include <iostream>
#include <glad/glad.h>
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

    // Start charging
    if (bothDown && !isCharging && InputManager::getCameraState() == CameraState::Free) {
        isCharging = true;
        chargeAmount = 0.0f;
        double mouseX;
        glfwGetCursorPos(window, &mouseX, &initialMouseY);
        lockedDirection = glm::normalize(camera.getFront());
        chargingViewLocked = true;
        InputManager::setCameraState(CameraState::Disabled);
    }

    // Update charge while holding
    if (isCharging && bothDown) {
        double mouseX, currentMouseY;
        glfwGetCursorPos(window, &mouseX, &currentMouseY);
        double deltaY = currentMouseY - initialMouseY;
        chargeAmount = std::clamp(static_cast<float>(deltaY) * chargeMultiplier, 0.0f, maxCharge);
    }

    // Release and launch
    if (isCharging && (!bothDown || InputManager::getCameraState() != CameraState::Disabled)) {
        if (chargeAmount > 0.0f) {
            glm::vec3 force = lockedDirection * chargeAmount * powerFactor;
            player.applyForce(force);
        }
        
        // Reset state
        isCharging = false;
        chargeAmount = 0.0f;
        lockedDirection = glm::vec3(0.0f);
        chargingViewLocked = false;
        
        // Only reset camera state if we were the ones who disabled it
        if (InputManager::getCameraState() == CameraState::Disabled) {
            InputManager::setCameraState(CameraState::Free);
        }
    }

    wasChargingLastFrame = isCharging;
}
