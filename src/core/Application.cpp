#include "Application.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "InputManager.h"

Application::Application()
    : camera(glm::vec3(0.0f, 0.5f, 3.0f)), player(&camera), renderer(camera)
{
    window = WindowManager::createWindow(800, 600, "16BitCraft");
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

    // Initialize other components
    renderer.initialize();
}

Application::~Application()
{
    glfwTerminate();
}

void Application::run()
{

    while (!glfwWindowShouldClose(window))
    {
        InputManager::processKeyboard(window, player);
        InputManager::handleDebugKeys(window);
        player.update();      // currently empty, but can be used for player updates         
        renderer.render();      // Render scene

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
