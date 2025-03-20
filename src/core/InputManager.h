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

private:
    static Camera *camera; // Static pointer to the camera object
};

extern bool firstMouse;
extern float lastX, lastY, yaw, pitch;

#endif
