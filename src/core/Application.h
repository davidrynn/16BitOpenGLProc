#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "ArmRenderer.h"
#include "WindowManager.h"
#include "Camera.h"
#include "LoadingBar.h"
#include "Renderer.h"
#include "Terrain.h"
#include "Player.h"
#include "SlingshotController.h"

class GrassSpawner;     
class GrassRenderer;

class Application {
public:
    Application();
    ~Application();
    void run();
    void updateGame(float deltaTime);

private:
    GLFWwindow* window;
    Camera camera;
    Renderer renderer;

    std::unique_ptr<GrassSpawner> grassSpawner;
    std::unique_ptr<GrassRenderer> grassRenderer;

    ArmRenderer armRenderer;
    Player player;
    std::shared_ptr<Terrain> terrain;
    SlingshotController slingshotController;
};