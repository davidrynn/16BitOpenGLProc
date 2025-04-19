#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Global variables for camera rotation
extern float yaw, pitch;
extern float lastX, lastY;
extern bool firstMouse;
extern glm::vec3 cameraFront;

class WindowManager
{
public:
    static GLFWwindow *createWindow(int width, int height, const char *title);
    static void processInput(GLFWwindow *window, glm::vec3 &cameraPos, glm::vec3 &cameraFront);
    static void mouseCallback(GLFWwindow *window, float xpos, float ypos);
    static void toggleCursor(GLFWwindow *window);

private:
    static bool cursorLocked;
};
#endif
