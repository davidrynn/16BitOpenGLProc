#include "InputManager.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Debug.h"
#include "WindowManager.h"

// Define all static members
Camera* InputManager::camera = nullptr;
ArmRenderer* InputManager::armRenderer = nullptr;
bool InputManager::firstMouse = true;
float InputManager::lastX = 400.0f;
float InputManager::lastY = 300.0f;
float InputManager::yaw = -90.0f;
float InputManager::pitch = 0.0f;
float InputManager::mouseSensitivity = 0.1f;
float InputManager::moveSpeed = 5.0f;
bool InputManager::punchTriggered = false;
bool InputManager::inventoryToggleTriggered = false;
CameraState InputManager::cameraState = CameraState::Free;

// Static key state tracking
static bool f1Pressed = false;
static bool f2Pressed = false;
static bool f3Pressed = false;
static bool f4Pressed = false;
static bool f5Pressed = false;
static bool spacePressed = false;
static bool shiftSpacePressed = false;
static bool punchPressed = false;

void InputManager::setCamera(Camera* cam)
{
    camera = cam;
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (camera == nullptr || cameraState != CameraState::Free) return;

    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
        return;
    }

    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(lastY - ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    if (camera) {
        camera->updateDirection(yaw, pitch);
    }

    // Handle right mouse button for clenching
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (armRenderer) {
            armRenderer->startClench();
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        if (armRenderer) {
            armRenderer->stopClench();
        }
    }
}

void InputManager::processKeyboard(GLFWwindow* window, Player& player)
{
    // Don't process movement if camera is disabled
    if (cameraState == CameraState::Disabled) return;

    const float currentSpeed = moveSpeed * (1.0f / 60.0f); // Normalize for 60 FPS

    // Basic movement (allowed in Free and Locked states)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.moveForward(currentSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.moveBackward(currentSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.moveLeft(currentSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.moveRight(currentSpeed);

    // Jump and Fly controls
    bool shiftHeld = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (shiftHeld && !shiftSpacePressed)
        {
            player.toggleFlyMode();
            shiftSpacePressed = true;
        }
        else if (!shiftHeld && !spacePressed)
        {
            player.jump();
            spacePressed = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        spacePressed = false;
        shiftSpacePressed = false;
    }

    // Vertical movement in fly mode
    if (player.isInFlyMode())
    {
        if (shiftHeld)
            player.moveDown(currentSpeed);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            player.moveUp(currentSpeed);
    }

    // Action controls
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !punchPressed)
    {
        punchTriggered = true;
        punchPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
    {
        punchPressed = false;
    }

    // Inventory toggle
    static bool inventoryKeyDown = false;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !inventoryKeyDown)
    {
        inventoryToggleTriggered = true;
        inventoryKeyDown = true;
        
        // Lock camera when inventory is opened
        setCameraState(CameraState::Locked);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)
    {
        inventoryKeyDown = false;
        
        // Unlock camera when inventory is closed
        if (cameraState == CameraState::Locked)
        {
            setCameraState(CameraState::Free);
        }
    }
}

void InputManager::setCameraState(CameraState newState)
{
    if (cameraState == newState) return;
    
    cameraState = newState;
    
    GLFWwindow* window = WindowManager::getWindow();
    if (!window) return;
    
    switch (newState)
    {
        case CameraState::Free:
            firstMouse = true;  // Reset first mouse flag to recenter
            break;
            
        case CameraState::Locked:
        case CameraState::Disabled:
            break;
    }
    
    Debug::log("Camera state changed to: " + 
               std::string(newState == CameraState::Free ? "Free" :
                         newState == CameraState::Locked ? "Locked" : "Disabled"));
}

void InputManager::resetMouseState(GLFWwindow* window)
{
    if (!window || !camera) return;
    
    // Get window center
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    lastX = static_cast<float>(width) / 2.0f;
    lastY = static_cast<float>(height) / 2.0f;
    
    // Update yaw/pitch based on current camera direction if camera exists
    if (camera) {
        glm::vec3 dir = camera->getFront();
        yaw = glm::degrees(atan2(dir.z, dir.x));
        pitch = glm::degrees(asin(dir.y));
    }
}

void InputManager::handleDebugKeys(GLFWwindow *window)
{
    // F1: Toggle Wireframe
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !f1Pressed)
    {
        Debug::toggleWireframe();
        Debug::log("Wireframe mode: " + std::string(Debug::isWireframeEnabled() ? "ON" : "OFF"));
        f1Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
        f1Pressed = false;

    // F2: Toggle FPS Display
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !f2Pressed)
    {
        Debug::toggleFPS();
        Debug::log("FPS Display: " + std::string(Debug::isFPSEnabled() ? "ON" : "OFF"));
        f2Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE)
        f2Pressed = false;

    // F3: Toggle Debug Info
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !f3Pressed)
    {
        Debug::toggleDebugInfo();
        Debug::log("Debug Info: " + std::string(Debug::isDebugInfoEnabled() ? "ON" : "OFF"));
        f3Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE)
        f3Pressed = false;

    // F4: Toggle Cursor Lock
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS && !f4Pressed)
    {
        WindowManager::toggleCursor(window);
        Debug::log("Cursor Lock: " + std::string(WindowManager::isCursorLocked() ? "ON" : "OFF"));
        f4Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_RELEASE)
        f4Pressed = false;

    // F5: Toggle Collision Boxes
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS && !f5Pressed)
    {
        Debug::toggleCollisionBoxes();
        Debug::log("Collision Boxes: " + std::string(Debug::isCollisionBoxesEnabled() ? "ON" : "OFF"));
        f5Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_RELEASE)
        f5Pressed = false;
}

bool InputManager::isKeyPressed(int key)
{
    GLFWwindow *window = glfwGetCurrentContext();
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void InputManager::resetActionTriggers()
{
    punchTriggered = false;
    inventoryToggleTriggered = false;
}

bool InputManager::isMouseButtonPressed(int button)
{
    GLFWwindow *window = WindowManager::getWindow();
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

glm::vec2 InputManager::getMousePosition()
{
    GLFWwindow *window = WindowManager::getWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (armRenderer) {
        // Update arm rotation by 5 degrees per scroll step
        armRenderer->updateRotation(static_cast<float>(yoffset) * 5.0f);
    }
}