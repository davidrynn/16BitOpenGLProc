#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class WindowManager {
public:
    static GLFWwindow* createWindow(int width, int height, const char* title);
    static void processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos); 
};
#endif
