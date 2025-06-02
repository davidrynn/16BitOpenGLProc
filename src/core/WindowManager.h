#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class WindowManager
{
public:
    static GLFWwindow *createWindow(int width, int height, const char *title);
    static GLFWwindow* getWindow();
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void toggleCursor(GLFWwindow *window);
    static bool isCursorLocked();

private:
    static bool cursorLocked;
    static GLFWwindow* window;
};
