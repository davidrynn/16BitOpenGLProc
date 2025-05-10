#include "Application.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <GLFW/glfw3.h>
#include "InputManager.h"
#include "ChunkConstants.h"

Application::Application()
    : terrain(std::make_shared<Terrain>()),
    camera(glm::vec3(0.0f, 0.5f, 3.0f)), 
    player(&camera), 
    renderer(camera)
{
    window = WindowManager::createWindow(1280, 720, "16BitCraft");
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Set OpenGL options
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Set input callbacks
    glfwSetKeyCallback(window, InputManager::keyCallback);
    glfwSetCursorPosCallback(window, InputManager::mouseCallback);

    // Set the camera in the InputManager
    InputManager::setCamera(&camera);

}

Application::~Application()
{
    glfwTerminate();
}

void Application::run()
{
    terrain = std::make_shared<Terrain>();
        // Show loading bar while terrain initializes incrementally
        {
            LoadingBar loadingBar("shaders/ui/loading.vert.glsl", "shaders/ui/loading.frag.glsl");
            loadingBar.initialize();
            auto noiseFactory = std::make_shared<TerrainNoiseFactory>();
            terrain->initialize(noiseFactory, [&](float progress) {
                glDisable(GL_DEPTH_TEST);
                loadingBar.render(progress, window);
                glEnable(GL_DEPTH_TEST);
                glfwSwapBuffers(window);
                glfwPollEvents();
            });
        }
        
        renderer.initialize(terrain);

    float terrainY = terrain->getHeightAt(player.camera->position.x, player.camera->position.z);
    player.camera->position.y = terrainY + 20.0f;
    float lastFrameTime = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window))
    {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        InputManager::processKeyboard(window, player);
        InputManager::handleDebugKeys(window);
        updateGame(deltaTime); 
        renderer.render(); // Render scene
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::updateGame(float deltaTime)
{
    float terrainY = terrain->getHeightAt(player.camera->position.x, player.camera->position.z);
    player.update(deltaTime, terrainY);
    terrain->updateChunksAroundPlayer(player.camera->position.x, player.camera->position.z);

    // // Get player position
    // float x = player.camera->position.x;
    // float z = player.camera->position.z;

    // // Calculate chunk coordinates (assuming each chunk is size `chunkSize`)
    // int chunkX = static_cast<int>(floor(x / ChunkConstants::SIZE));
    // int chunkZ = static_cast<int>(floor(z / ChunkConstants::SIZE));

    // bool surrounded = terrain->hasChunksOnAllSides(chunkX, chunkZ);

    // std::cout << "Player Position: (" << x << ", " << z << ") | "
    //           << "Surrounded: " << (surrounded ? "Yes" : "No") << std::endl;
}
