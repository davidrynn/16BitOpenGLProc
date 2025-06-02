#include "Application.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include "InputManager.h"
#include "ChunkConstants.h"
#include "TerrainConstants.h"
#include "Config.h"
#include "Debug.h"
#include "WindowManager.h"
#include "TerrainThreadPool.h"

Application::Application()
    : shouldClose(false)
    , lastFrameTime(0.0f)
    , deltaTime(0.0f)
    , globTriggered(false)
    , globHoldTime(0.0f)
    , lastValidHit(0.0f)
    , fKeyMarkerActive(false)
    , windowWidth(0)
    , windowHeight(0)
    , window(nullptr)
    , terrainThreadPool(std::make_unique<TerrainThreadPool>())
{
    // Load configuration first
    Config& config = Config::getInstance();
    windowWidth = config.window.width;
    windowHeight = config.window.height;

    if (!initializeGLFW()) {
        Debug::logError("Failed to initialize GLFW");
        throw std::runtime_error("Failed to initialize GLFW");
    }

    if (!initializeGLAD()) {
        Debug::logError("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    try {
        // Initialize core components
        camera = std::make_unique<Camera>();
        player = std::make_unique<Player>(camera.get());
        loadingBar = std::make_unique<LoadingBar>("shaders/ui/loading.vert", "shaders/ui/loading.frag");
        raycaster = std::make_unique<Raycaster>();
        
        // Initialize terrain with loading bar
        initializeTerrain();
        
        // Initialize renderer after terrain
        renderer = std::make_unique<Renderer>(*camera);
        renderer->initialize(terrain);
        
        // Initialize game systems
        slingshotController = std::make_unique<SlingshotController>();
        
        // Initialize input system
        InputManager::setCamera(camera.get());
        InputManager::setMouseSensitivity(config.input.mouseSensitivity);
        InputManager::setMoveSpeed(config.input.moveSpeed);
        InputManager::setCameraState(CameraState::Free);  // Set initial state to Free
        
        // Set up callbacks
        initializeCallbacks();
        
        // Apply vsync setting
        glfwSwapInterval(config.window.vsync ? 1 : 0);
        
        Debug::log("Application initialized successfully");
    } catch (const std::exception& e) {
        Debug::logError("Failed to initialize application components: " + std::string(e.what()));
        cleanupResources();
        throw;
    }
}

Application::~Application()
{
    cleanupResources();
}

void Application::cleanupResources()
{
    // Release resources in reverse order of acquisition
    renderer.reset();
    player.reset();
    camera.reset();
    
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
    
    Debug::log("Application resources cleaned up");
}

bool Application::initializeGLFW()
{
    Debug::log("Initializing GLFW...");
    if (!glfwInit()) {
        Debug::logError("glfwInit failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Debug::log("Creating window...");
    window = glfwCreateWindow(windowWidth, windowHeight, "16BitCraft", nullptr, nullptr);
    if (!window) {
        Debug::logError("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    
    // Initialize cursor in disabled (captured) mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    
    Debug::log("GLFW initialization successful");
    return true;
}

bool Application::initializeGLAD()
{
    Debug::log("Initializing GLAD...");
    bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!result) {
        Debug::logError("GLAD initialization failed");
    } else {
        Debug::log("GLAD initialization successful");
    }
    return result;
}

void Application::initializeCallbacks()
{
    // Set up GLFW callbacks
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(w));
        app->windowWidth = width;
        app->windowHeight = height;
        app->updateProjectionMatrix();
        glViewport(0, 0, width, height);
    });

    glfwSetCursorPosCallback(window, WindowManager::mouseCallback);
    glfwSetKeyCallback(window, InputManager::keyCallback);
    glfwSetScrollCallback(window, InputManager::scrollCallback);
    
    // Set the ArmRenderer in InputManager after renderer is initialized
    if (renderer) {
        InputManager::setArmRenderer(renderer->getArmRenderer());
    }
}

void Application::updateProjectionMatrix()
{
    if (renderer) {
        float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
        renderer->updateProjectionMatrix(aspectRatio);
    }
}

void Application::handleInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        shouldClose = true;
    }

    // Handle regular input
    InputManager::processKeyboard(window, *player);
    InputManager::handleDebugKeys(window);
}

void Application::initializeTerrain() {
    // Create terrain with thread pool
    terrain = std::make_shared<Terrain>(*terrainThreadPool);
    loadingBar->initialize();
    
    auto noiseFactory = std::make_shared<TerrainNoiseFactory>();
    terrain->initialize(noiseFactory, [this](float progress) {
        glDisable(GL_DEPTH_TEST);
        loadingBar->render(progress, window);
        glEnable(GL_DEPTH_TEST);
        glfwSwapBuffers(window);
        glfwPollEvents();
    });
    
    // Set initial player position based on terrain height
    if (player && camera) {
        float terrainY = terrain->getHeightAt(camera->getPosition().x, camera->getPosition().z);
        glm::vec3 newPos = camera->getPosition();
        newPos.y = terrainY + 20.0f;
        camera->setPosition(newPos);
    }
}

void Application::updateGame(float dt) {
    if (!player || !camera) return;
    
    // Update player with terrain height
    glm::vec3 pos = camera->getPosition();
    float terrainHeight = terrain ? terrain->getHeightAt(pos.x, pos.z) : 0.0f;
    player->update(dt, terrainHeight);
    
    // Update slingshot controller
    if (slingshotController) {
        slingshotController->update(window, *camera, *player);
    }
    
    // Update arm renderer
    if (renderer) {
        renderer->getArmRenderer()->update(dt);
    }
    
    // Handle arm punch (P key)
    if (InputManager::punchTriggered && renderer) {
        renderer->triggerArmPunch();
    }
    
    // Handle debug marker (F key)
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyMarkerActive) {
        auto hit = Raycaster::raycastToTerrain(*camera, terrain);
        if (hit) {
            lastValidHit = *hit;
            renderer->setDebugMarkerPosition(lastValidHit);
            renderer->showDebugMarker(true);
        }
        fKeyMarkerActive = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        fKeyMarkerActive = false;
        renderer->showDebugMarker(false);
    }
    
    // Handle earth glob creation
    bool rightMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    
    if (rightMouseDown && !globTriggered) {
        // Start timing the hold
        globTriggered = true;
        globHoldTime = 0.0f;
    }
    else if (rightMouseDown && globTriggered) {
        // Accumulate hold time
        globHoldTime += dt;
        
        // After 3 seconds of holding, perform raycast and create glob
        if (globHoldTime >= 3.0f) {
            auto hit = Raycaster::raycastToTerrain(*camera, terrain);
            if (hit) {
                lastValidHit = *hit;
                renderer->beginTerrainLift(lastValidHit);
                globTriggered = false;
                globHoldTime = 0.0f;
            }
        }
    }
    else if (!rightMouseDown && globTriggered) {
        // Reset if released before threshold
        globTriggered = false;
        globHoldTime = 0.0f;
    }
    
    // Update terrain with less aggressive spiral pattern
    static float chunkUpdateAccumulator = 0.0f;
    static const float chunkUpdateInterval = 0.25f; // 250ms between chunk updates to reduce load
    
    chunkUpdateAccumulator += dt;
    if (chunkUpdateAccumulator >= chunkUpdateInterval) {
        chunkUpdateAccumulator = 0.0f;
        
        glm::vec3 pos = camera->getPosition();
        terrain->updateChunksAroundPlayer(pos.x, pos.z);
    }
    
    // Process chunk uploads with timing control
    if (terrainThreadPool) {
        terrainThreadPool->processUploads();
    }
    
    if (renderer) {
        renderer->updateTerrainManipulator(dt);
    }
    
    // Reset input flags
    InputManager::resetActionTriggers();
}

void Application::run() {
    Debug::log("Starting main loop...");
    
    // Track frame timing for smooth updates
    const float targetFrameTime = 1.0f / 60.0f;  // Target 60 FPS
    float accumulator = 0.0f;
    
    while (!shouldClose && !glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;
        
        // Cap deltaTime to prevent large jumps
        deltaTime = std::min(deltaTime, 0.1f);
        accumulator += deltaTime;
        
        handleInput();
        
        // Fixed time step update
        while (accumulator >= targetFrameTime) {
            updateGame(targetFrameTime);
            accumulator -= targetFrameTime;
        }
        
        // Process chunk uploads with timing control
        terrainThreadPool->processUploads();
        
        if (renderer) {
            renderer->render();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Handle FPS limiting if vsync is off
        Config& config = Config::getInstance();
        if (!config.window.vsync && config.graphics.maxFPS > 0) {
            float frameTime = 1.0f / config.graphics.maxFPS;
            float elapsed = static_cast<float>(glfwGetTime()) - currentFrame;
            if (elapsed < frameTime) {
                float sleepTime = (frameTime - elapsed) * 1000.0f;
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime)));
            }
        }
        
        // Display FPS and chunk info if enabled
        if (Debug::isFPSEnabled()) {
            static int frameCount = 0;
            static float timeAccumulator = 0.0f;
            
            frameCount++;
            timeAccumulator += deltaTime;
            
            if (timeAccumulator >= 1.0f) {
                std::string debugInfo = "FPS: " + std::to_string(frameCount);
                debugInfo += " | Pending Uploads: " + std::to_string(terrainThreadPool->getPendingUploadCount());
                Debug::log(debugInfo);
                frameCount = 0;
                timeAccumulator = 0.0f;
            }
        }
    }
    Debug::log("Main loop ended. shouldClose=" + std::to_string(shouldClose) + 
               ", windowShouldClose=" + std::to_string(glfwWindowShouldClose(window)));
}
