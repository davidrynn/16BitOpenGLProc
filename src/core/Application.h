#ifndef APPLICATION_H
#define APPLICATION_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "WindowManager.h"
#include "Camera.h"
#include "Renderer.h"
#include "Terrain.h"
#include "Player.h"

class Application {
public:
    Application();
    ~Application();
    void run();
private:
    GLFWwindow* window;
    Camera camera;
    Renderer renderer;
    Player player;
};

#endif
