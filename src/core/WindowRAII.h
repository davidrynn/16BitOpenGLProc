#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class WindowRAII {
    GLFWwindow* window;
public:
    WindowRAII(int width, int height, const char* title) {
        // Initialize GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        
        // Set OpenGL version and core profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        
        // Make the window's context current
        glfwMakeContextCurrent(window);
        
        // Enable raw mouse motion if available
        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }
    
    ~WindowRAII() {
        if (window) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }
    
    GLFWwindow* get() const { return window; }
    
    // Prevent copying
    WindowRAII(const WindowRAII&) = delete;
    WindowRAII& operator=(const WindowRAII&) = delete;
}; 