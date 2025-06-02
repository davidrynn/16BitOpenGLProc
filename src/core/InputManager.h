#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Player.h"
#include "ArmRenderer.h"
#include <glm/glm.hpp>

// Similar to Swift's enum
enum class CameraState {
    Free,       // Normal camera movement
    Locked,     // Camera locked but still visible (during UI)
    Disabled    // Camera completely disabled
};

class InputManager
{
public:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void processKeyboard(GLFWwindow *window, Player &player);
    static void setCamera(Camera *cam);
    static void setArmRenderer(ArmRenderer *arm) { armRenderer = arm; }
    static void handleDebugKeys(GLFWwindow* window);
    static bool isKeyPressed(int key);
    static bool isMouseButtonPressed(int button);
    static glm::vec2 getMousePosition();

    static void resetActionTriggers(); // Clear after use

    // Camera state management (like Swift's property observers)
    static void setCameraState(CameraState newState);
    static CameraState getCameraState() { return cameraState; }

    // Mouse sensitivity (like Swift's properties)
    static void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
    static float getMouseSensitivity() { return mouseSensitivity; }

    // Movement speed
    static void setMoveSpeed(float speed) { moveSpeed = speed; }
    static float getMoveSpeed() { return moveSpeed; }

    // Action flags (similar to Swift's @Published properties in SwiftUI)
    static bool punchTriggered;
    static bool inventoryToggleTriggered;

    // Camera control getters
    static float getYaw() { return yaw; }
    static float getPitch() { return pitch; }
    static float getLastX() { return lastX; }
    static float getLastY() { return lastY; }
    static bool getFirstMouse() { return firstMouse; }
    static void setYaw(float y) { yaw = y; }
    static void setPitch(float p) { pitch = p; }
    static void setLastX(float x) { lastX = x; }
    static void setLastY(float y) { lastY = y; }
    static void setFirstMouse(bool first) { firstMouse = first; }

private:
    static Camera *camera;
    static ArmRenderer *armRenderer;
    static bool firstMouse;
    static float lastX, lastY, yaw, pitch;
    static float mouseSensitivity;  
    static float moveSpeed;         
    static CameraState cameraState;

    static void handleWireframeKey(GLFWwindow *window);
    static void handleReloadShadersKey(GLFWwindow *window);
    static void handleShowFPSKey(GLFWwindow *window);
    static void handleToggleCursorKey(GLFWwindow *window);

    // Helper method to reset mouse state when changing camera states
    static void resetMouseState(GLFWwindow* window);
};
