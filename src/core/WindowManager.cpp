#include "WindowManager.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "InputManager.h"

GLFWwindow* WindowManager::window = nullptr;

GLFWwindow* WindowManager::createWindow(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // OpenGL version 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize cursor in disabled (captured) mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    InputManager::setCameraState(CameraState::Free);

    return window;
}

GLFWwindow* WindowManager::getWindow() {
    return window;
}

void WindowManager::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Forward to InputManager's mouseCallback which handles camera movement
    InputManager::mouseCallback(window, xpos, ypos);
}

void WindowManager::toggleCursor(GLFWwindow* window) {
    int currentMode = glfwGetInputMode(window, GLFW_CURSOR);
    if (currentMode == GLFW_CURSOR_DISABLED) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        InputManager::setCameraState(CameraState::Locked);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        InputManager::setCameraState(CameraState::Free);
        // Reset mouse position to center when re-capturing
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glfwSetCursorPos(window, static_cast<double>(width) / 2.0, static_cast<double>(height) / 2.0);
    }
}

bool WindowManager::isCursorLocked() {
    GLFWwindow* window = getWindow();
    if (!window) return false;
    return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

