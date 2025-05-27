#include "InputManager.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Debug.h"
#include "WindowManager.h"

// Define the global variables
bool firstMouse = true;
float lastX = 400.0f, lastY = 300.0f;
float yaw = -90.0f; // Initialize yaw to -90.0 degrees to look forward
float pitch = 0.0f; // Initialize pitch to 0.0 degrees
static bool cameraFrozen = false;

// Debug variables
bool wireframeEnabled = false;
bool f1Pressed = false;
bool f2Pressed = false;
bool f3Pressed = false;
bool f4Pressed = false;
float lastTime = static_cast<float>(glfwGetTime());
int frameCount = 0;

// keyboard state variables
bool spacePressed = false;
bool shiftSpacePressed = false;
bool punchPressed = false;
bool InputManager::punchTriggered = false;
bool InputManager::inventoryToggleTriggered = false;

// Define the static camera pointer
Camera *InputManager::camera = nullptr;

void InputManager::setCamera(Camera *cam)
{
    camera = cam;
}

void InputManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void InputManager::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{

    if (cameraFrozen)
        return;
    float xposF = static_cast<float>(xpos);
    float yposF = static_cast<float>(ypos);
    if (firstMouse)
    {
        lastX = xposF;
        lastY = yposF;
        firstMouse = false;
    }

    float xoffset = xposF - lastX;
    float yoffset = lastY - yposF; // Reversed since y-coordinates go from bottom to top

    lastX = xposF;
    lastY = yposF;

    const float sensitivity = 0.1f; // Adjust sensitivity as needed
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to avoid flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Update camera direction based on yaw and pitch
    if (camera)
    {
        camera->updateDirection(yaw, pitch);
    }
}

void InputManager::processKeyboard(GLFWwindow *window, Player &player)
{
    const float cameraSpeed = 0.15f; // Adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.moveForward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.moveBackward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.moveLeft(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.moveRight(cameraSpeed);

    // Jump and Fly
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

    if (shiftHeld && player.isInFlyMode())
        player.moveDown(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && player.isInFlyMode())
        player.moveUp(cameraSpeed);

    // --- Punch action (P key) ---
    static bool punchKeyDown = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !punchKeyDown)
    {
        punchTriggered = true;
        punchKeyDown = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
    {
        punchKeyDown = false;
    }

    // --- Inventory toggle (I key) ---
    static bool inventoryKeyDown = false;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !inventoryKeyDown)
    {
        inventoryToggleTriggered = true;
        inventoryKeyDown = true;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)
    {
        inventoryKeyDown = false;
    }
}

void InputManager::handleDebugKeys(GLFWwindow *window)
{
    handleWireframeKey(window);
    handleReloadShadersKey(window);
    handleShowFPSKey(window);
    handleToggleCursorKey(window);
}

void InputManager::handleWireframeKey(GLFWwindow *window)
{
    // --- F1: Toggle Wireframe ---
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !f1Pressed)
    {
        wireframeEnabled = !wireframeEnabled;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
        std::cout << "Wireframe mode: " << (wireframeEnabled ? "ON" : "OFF") << std::endl;
        f1Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
    {
        f1Pressed = false;
    }
}

void InputManager::handleReloadShadersKey(GLFWwindow *window)
{

    // --- F2: Reload Shaders (optional placeholder) ---
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !f2Pressed)
    {
        std::cout << "Reloading shaders... (implement if needed)" << std::endl;
        f2Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE)
    {
        f2Pressed = false;
    }
}

void InputManager::handleShowFPSKey(GLFWwindow *window)
{

    // --- F3: Show FPS ---
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !f3Pressed)
    {
        float currentTime = static_cast<float>(glfwGetTime());
        frameCount++;

        if (currentTime - lastTime >= 1.0f)
        {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        f3Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE)
    {
        f3Pressed = false;
    }
}

void InputManager::handleToggleCursorKey(GLFWwindow *window)
{
    // --- F4: Toggle Cursor ---
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS && !f4Pressed)
    {
        WindowManager::toggleCursor(window); // Toggle the cursor
        f4Pressed = true;                    // Mark as pressed
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_RELEASE)
    {
        f4Pressed = false; // Reset when the key is released
    }
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

void InputManager::freezeCamera()
{
    cameraFrozen = true;
}

void InputManager::unfreezeCamera()
{
    cameraFrozen = false;

    if (camera)
    {
        glm::vec3 dir = camera->front;
        yaw = glm::degrees(atan2(dir.z, dir.x));
        pitch = glm::degrees(asin(dir.y));

        // Reset last mouse position to current to prevent jump
        GLFWwindow *window = WindowManager::getWindow();
        if (window)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            firstMouse = true;
        }
    }
}