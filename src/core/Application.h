#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Camera.h"
#include "LoadingBar.h"
#include "Raycaster.h"
#include "Renderer.h"
#include "Terrain.h"
#include "Player.h"
#include "SlingshotController.h"
#include "Config.h"
#include "TerrainThreadPool.h"

class Application {
public:
    Application();
    ~Application();
    
    // Delete copy constructor and assignment operator
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();
    void stop() { shouldClose = true; }
    
    // Add getter for renderer
    Renderer* getRenderer() { return renderer.get(); }

private:
    bool initializeGLFW();
    bool initializeGLAD();
    void initializeCallbacks();
    void initializeTerrain();
    void updateProjectionMatrix();
    void handleInput();
    void updateGame(float dt);
    void updateChunksAroundPlayer();
    void cleanupResources();

    // Core systems
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Player> player;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Config> config;
    std::shared_ptr<Terrain> terrain;
    std::unique_ptr<TerrainThreadPool> terrainThreadPool;
    
    // Game systems
    std::unique_ptr<SlingshotController> slingshotController;
    std::unique_ptr<LoadingBar> loadingBar;
    std::unique_ptr<Raycaster> raycaster;

    // State
    bool shouldClose;
    int windowWidth;
    int windowHeight;
    float lastFrameTime;
    float deltaTime;
    bool globTriggered;
    float globHoldTime;
    glm::vec3 lastValidHit;
    bool fKeyMarkerActive;
    
    // Window is managed by GLFW, we don't own it
    GLFWwindow* window;
};