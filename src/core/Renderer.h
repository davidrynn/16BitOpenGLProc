#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Shader.h"
#include "Terrain.h"
#include "SkyGradient.h"
#include "DebugMarker.h"
#include "TerrainManipulator.h"
#include "ReticleRenderer.h"
#include "GrassSpawner.h"
#include "GrassRenderer.h"
#include "GridRenderer.h"
#include "IArmRenderer.h"
#include "ArmRenderer.h"
#include "TriArmRenderer.h"
#include "ModelArmRenderer.h"

enum class ArmRendererType {
    Standard,
    Triangular,
    Model
};

class Renderer {
public:
    explicit Renderer(Camera& camera);
    ~Renderer();

    // Delete copy constructor and assignment operator
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Core rendering
    void initialize(std::shared_ptr<Terrain> terrainPtr);
    void render();
    void updateProjectionMatrix(float aspectRatio);
    
    // Terrain management
    void updateChunksAroundPosition(float x, float z);
    void beginTerrainLift(const glm::vec3& position);
    void updateTerrainManipulator(float deltaTime);
    
    // Debug visualization
    void setDebugMarkerPosition(const glm::vec3& position);
    void showDebugMarker(bool show);
    
    // Game actions
    void triggerArmPunch();
    void setArmRendererType(ArmRendererType type);

    IArmRenderer* getArmRenderer() { return armRenderer.get(); }

private:
    void initializeOpenGLState();
    void cleanupOpenGLResources();
    void checkGLError(const char* operation);

    // OpenGL resources
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    // Components
    Camera& camera;
    std::unique_ptr<Shader> shader;
    std::shared_ptr<Terrain> terrain;
    std::unique_ptr<SkyGradient> skyGradient;
    std::unique_ptr<DebugMarker> debugMarker;
    std::unique_ptr<TerrainManipulator> terrainManipulator;
    std::unique_ptr<ReticleRenderer> reticleRenderer;
    std::unique_ptr<GrassSpawner> grassSpawner;
    std::unique_ptr<GrassRenderer> grassRenderer;
    std::unique_ptr<GridRenderer> gridRenderer;
    std::unique_ptr<IArmRenderer> armRenderer;

    // Matrices
    glm::mat4 projectionMatrix;
    
    // State tracking
    bool initialized;
};
