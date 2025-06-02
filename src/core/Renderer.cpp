#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "Debug.h"
#include "Shader.h"
#include "Camera.h"
#include "InputManager.h"

Renderer::Renderer(Camera &camera)
    : VAO(0), VBO(0), EBO(0), camera(camera), shader(nullptr), initialized(false)
{
    projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 1000.0f);
}

Renderer::~Renderer()
{
    cleanupOpenGLResources();
}

void Renderer::cleanupOpenGLResources()
{
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (EBO != 0) glDeleteBuffers(1, &EBO);
}

void Renderer::initialize(std::shared_ptr<Terrain> terrainPtr)
{
    try {
        terrain = terrainPtr;
        
        // Create and store shader
        shader = std::make_unique<Shader>("shaders/terrain.vert", "shaders/terrain.frag");
        shader->use(); // Use the shader once at initialization
        shader->setVec3("lightDir", glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
        shader->setVec3("baseColor", glm::vec3(0.4f, 0.8f, 0.4f)); // grassy color

        // Initialize sky gradient first since it's our background
        skyGradient = std::make_unique<SkyGradient>();

        // Set up OpenGL state
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);  // Standard depth testing
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Check for OpenGL errors
        checkGLError("initialize");

        // Initialize other renderers
        debugMarker = std::make_unique<DebugMarker>();
        debugMarker->initialize();

        terrainManipulator = std::make_unique<TerrainManipulator>();
        terrainManipulator->initialize(terrain);

        reticleRenderer = std::make_unique<ReticleRenderer>();
        reticleRenderer->initialize();

        grassSpawner = std::make_unique<GrassSpawner>(terrain, 2);
        grassSpawner->generate();

        grassRenderer = std::make_unique<GrassRenderer>();
        grassRenderer->initialize();
        grassRenderer->update(grassSpawner->getGrassPositions());

        gridRenderer = std::make_unique<GridRenderer>(200, 1.0f);
        armRenderer = std::make_unique<ArmRenderer>();
        armRenderer->initialize();

        initialized = true;
        Debug::log("Renderer initialized successfully");
    } catch (const std::exception& e) {
        Debug::logError("Failed to initialize renderer: " + std::string(e.what()));
        throw;
    }
}

void Renderer::render()
{
    if (!initialized) {
        Debug::logWarning("Attempting to render before initialization");
        return;
    }

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render sky gradient (background)
    glDisable(GL_DEPTH_TEST);
    skyGradient->render();
    glEnable(GL_DEPTH_TEST);
    
    // Get the current framebuffer size
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    
    // Use consistent near/far planes across all rendering
    const float nearPlane = 0.1f;
    const float farPlane = 1000.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                          (float)width / (float)height,
                                          nearPlane, farPlane);

    // Set up camera view matrix
    glm::mat4 view = camera.getViewMatrix();

    // Render terrain chunks
    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    // Ensure proper depth testing state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    // Render terrain chunks
    for (const auto& pair : terrain->getChunks()) {
        const std::shared_ptr<Chunk>& chunk = pair.second;
        if (chunk && chunk->isUploaded()) {
            chunk->render(*shader);
        }
    }

    // Render grass with proper depth testing
    if (grassRenderer) {
        float time = static_cast<float>(glfwGetTime());
        grassRenderer->render(view, projection, time);
    }

    // Render arm with proper depth
    armRenderer->render(camera, projection);

    // Render debug elements
    if (terrainManipulator) {
        terrainManipulator->render(view, projection);
    }
    if (debugMarker) {
        debugMarker->render(view, projection);
    }

    // Render UI elements last
    if (reticleRenderer) {
        glDisable(GL_DEPTH_TEST);
        reticleRenderer->render();
        glEnable(GL_DEPTH_TEST);
    }

    // Draw debug grid if enabled
    if (Debug::isWireframeEnabled() && gridRenderer) {
        glDisable(GL_DEPTH_TEST);
        gridRenderer->render(view, projection, glm::vec3(0.3f));
        glEnable(GL_DEPTH_TEST);
    }

    checkGLError("render");
}

void Renderer::updateProjectionMatrix(float aspectRatio)
{
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
}

void Renderer::updateChunksAroundPosition(float x, float z)
{
    if (terrain)
    {
        terrain->updateChunksAroundPlayer(x, z);
    }
}

void Renderer::beginTerrainLift(const glm::vec3& position)
{
    if (terrainManipulator)
    {
        terrainManipulator->beginLift(position);
    }
}

void Renderer::updateTerrainManipulator(float deltaTime)
{
    if (terrainManipulator)
    {
        terrainManipulator->update(deltaTime);
    }
}

void Renderer::setDebugMarkerPosition(const glm::vec3& position)
{
    if (debugMarker)
    {
        debugMarker->setPosition(position);
        debugMarker->show();
    }
}

void Renderer::showDebugMarker(bool show)
{
    if (debugMarker)
    {
        if (show)
        {
            debugMarker->show();
        }
        else
        {
            debugMarker->hide();
        }
    }
}

void Renderer::triggerArmPunch()
{
    if (armRenderer)
    {
        armRenderer->triggerPunch();
    }
}

void Renderer::checkGLError(const char* operation)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::string errorStr;
        switch (error)
        {
            case GL_INVALID_ENUM:       errorStr = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:      errorStr = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:  errorStr = "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:      errorStr = "GL_OUT_OF_MEMORY"; break;
            default:                    errorStr = "UNKNOWN"; break;
        }
        Debug::logError("OpenGL error after " + std::string(operation) + ": " + errorStr);
    }
}