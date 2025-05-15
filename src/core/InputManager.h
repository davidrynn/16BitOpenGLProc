#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Player.h"

class InputManager
{
public:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void processKeyboard(GLFWwindow *window, Player &player);
    static void setCamera(Camera *cam);
    static void handleDebugKeys(GLFWwindow* window);
       // These flags can be read/reset from Application
    static bool punchTriggered;
    static bool inventoryToggleTriggered;
    static bool isMouseButtonPressed(int button);
    static glm::vec2 getMousePosition();

    static void resetActionTriggers(); // Clear after use

    static void freezeCamera();
    static void unfreezeCamera();

private:
    static Camera *camera; // Static pointer to the camera object
    static void handleWireframeKey(GLFWwindow *window);
    static void handleReloadShadersKey(GLFWwindow *window);
    static void handleShowFPSKey(GLFWwindow *window);
    static void handleToggleCursorKey(GLFWwindow *window);
};

extern bool firstMouse;
extern float lastX, lastY, yaw, pitch;

#endif
