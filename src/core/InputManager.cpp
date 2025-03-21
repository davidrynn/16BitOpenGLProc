#include "InputManager.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Define the global variables
bool firstMouse = true;
float lastX = 400.0f, lastY = 300.0f;
float yaw = -90.0f; // Initialize yaw to -90.0 degrees to look forward
float pitch = 0.0f; // Initialize pitch to 0.0 degrees

// Debug variables
bool wireframeEnabled = false;
bool f1Pressed = false;
bool f2Pressed = false;
bool f3Pressed = false;

// Define the static camera pointer
Camera* InputManager::camera = nullptr;

void InputManager::setCamera(Camera* cam) {
    camera = cam;
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

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
    if (camera) {
        camera->updateDirection(yaw, pitch);
    }
}

void InputManager::processKeyboard(GLFWwindow* window, Player& player)
{
    const float cameraSpeed = 0.05f; // Adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.moveForward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.moveBackward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.moveLeft(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.moveRight(cameraSpeed);
}

void InputManager::handleDebugKeys(GLFWwindow* window) {
    // --- F1: Toggle Wireframe ---
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !f1Pressed) {
        wireframeEnabled = !wireframeEnabled;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
        std::cout << "Wireframe mode: " << (wireframeEnabled ? "ON" : "OFF") << std::endl;
        f1Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
        f1Pressed = false;
    }

    // --- F2: Reload Shaders (optional placeholder) ---
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !f2Pressed) {
        std::cout << "Reloading shaders... (implement if needed)" << std::endl;
        f2Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) {
        f2Pressed = false;
    }

    // --- F3: Show FPS ---
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !f3Pressed) {
        static float lastTime = glfwGetTime();
        static int frameCount = 0;

        float currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - lastTime >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        f3Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE) {
        f3Pressed = false;
    }
}