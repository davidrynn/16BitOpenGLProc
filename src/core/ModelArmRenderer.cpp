#include "ModelArmRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Debug.h"

void ModelArmRenderer::initialize()
{
    Debug::log("Starting ModelArmRenderer initialization...");

    armShader = std::make_unique<Shader>("shaders/model.vert", "shaders/model.frag");
    armShader->use();

    Debug::log("Loading triangular arm model...");
    try
    {
        // Load the triangular arm model
        armModel = std::make_unique<Model>("models/triangleArm.glb");
        Debug::log("Model loaded successfully");
        
        // Initialize bone transforms
        boneTransforms.resize(MAX_BONES, glm::mat4(1.0f));
        
        // Add debug information about the model
        Debug::log("Model path: models/triangleArm.glb");
        Debug::log("Debug mode enabled: " + std::string(debugMode ? "true" : "false"));
        Debug::log("Initial model scale: " + std::to_string(modelScale));
        Debug::log("Number of bone transforms: " + std::to_string(boneTransforms.size()));
        
        // Enable debug mode by default to help with initial positioning
        debugMode = true;
    }
    catch (const std::exception &e)
    {
        Debug::logError("Failed to load model: " + std::string(e.what()));
        throw;
    }

    Debug::log("ModelArmRenderer initialization complete.");
}

void ModelArmRenderer::update(float deltaTime)
{
    const float CLENCH_THRESHOLD = 0.2f;

    if (isHolding)
    {
        holdTime += deltaTime;
        if (holdTime >= CLENCH_THRESHOLD)
        {
            clenching = true;
        }
    }
    else
    {
        holdTime = 0.0f;
        clenching = false;
    }

    // Update clenching animation
    if (clenching && clenchAmount < 1.0f)
    {
        clenchAmount = std::min(clenchAmount + deltaTime * 2.0f, 1.0f);
    }
    else if (!clenching && clenchAmount > 0.0f)
    {
        clenchAmount = std::max(clenchAmount - deltaTime * 2.0f, 0.0f);
    }
}

void ModelArmRenderer::triggerPunch()
{
    if (!punching)
    {
        punching = true;
        punchStartTime = static_cast<float>(glfwGetTime());
    }
}

void ModelArmRenderer::updateBoneTransforms()
{
    // Calculate bone transforms based on animation state
    float clenchRotation = clenchAmount * 45.0f; // Up to 45 degrees rotation for clenching
    float punchTranslation = 0.0f;
    
    if (punching) {
        float currentTime = static_cast<float>(glfwGetTime());
        float timeSincePunch = currentTime - punchStartTime;
        
        if (timeSincePunch < 0.2f) {
            punchTranslation = -0.3f * (timeSincePunch / 0.2f);
        }
        else if (timeSincePunch < 0.4f) {
            punchTranslation = -0.3f * (1.0f - ((timeSincePunch - 0.2f) / 0.2f));
        }
        else {
            punching = false;
        }
    }

    // Reset all transforms to identity
    for (auto& transform : boneTransforms) {
        transform = glm::mat4(1.0f);
    }

    // Base rotations for the arm to match the standard arm renderer
    glm::mat4 baseRotation = glm::mat4(1.0f);
    baseRotation = glm::rotate(baseRotation, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Slight lean
    baseRotation = glm::rotate(baseRotation, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Reach hand down on x axis
    baseRotation = glm::rotate(baseRotation, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate palm up on y axis

    // Apply base rotation to all bones
    for (auto& transform : boneTransforms) {
        transform = baseRotation;
    }

    if (debugMode) {
        Debug::log("Updating bone transforms:");
        Debug::log("Clench amount: " + std::to_string(clenchAmount));
        Debug::log("Clench rotation: " + std::to_string(clenchRotation));
        Debug::log("Punch translation: " + std::to_string(punchTranslation));
        Debug::log("First bone transform translation: (" +
                   std::to_string(boneTransforms[0][3][0]) + ", " +
                   std::to_string(boneTransforms[0][3][1]) + ", " +
                   std::to_string(boneTransforms[0][3][2]) + ")");
    }
}

void ModelArmRenderer::render(const Camera& camera, const glm::mat4& projection)
{
    if (!armModel || !armShader) {
        Debug::logError("Model or shader not initialized!");
        return;
    }

    armShader->use();

    // Set up lighting uniforms
    armShader->setVec3("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
    armShader->setVec3("viewPos", camera.getPosition());

    // Set up matrices
    armShader->setMat4("projection", projection);
    armShader->setMat4("view", camera.getViewMatrix());

    glm::mat4 model = glm::mat4(1.0f);
    
    // Position model to the right side of the camera
    glm::vec3 cameraPos = camera.getPosition();
    glm::vec3 cameraRight = camera.getRight();
    glm::vec3 cameraFront = camera.getFront();
    
    // Position the arm to the right and slightly in front
    modelPosition = cameraPos + (cameraRight * 0.3f) + (cameraFront * 0.5f);
    modelPosition.y -= 0.15f; // Slightly below eye level
    
    // First translate to position relative to camera
    model = glm::translate(model, modelPosition);
    
    // Apply camera orientation to keep arm aligned with view
    glm::mat4 viewRotation = glm::mat4(glm::mat3(camera.getViewMatrix())); // Extract rotation from view matrix
    model = model * glm::inverse(viewRotation);
    
    // Apply base arm orientation
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Face forward
    
    // Apply manual rotation from mouse wheel
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Scale the model
    model = glm::scale(model, glm::vec3(0.05f));
    
    armShader->setMat4("model", model);

    // Update and set bone transforms
    updateBoneTransforms();
    for (size_t i = 0; i < boneTransforms.size(); ++i) {
        std::string name = "bones[" + std::to_string(i) + "]";
        armShader->setMat4(name.c_str(), boneTransforms[i]);
    }

    // Debug information
    Debug::log("Drawing model with:");
    Debug::log("Model scale: " + std::to_string(0.05f));
    Debug::log("Model position: " + std::to_string(modelPosition.x) + ", " + 
               std::to_string(modelPosition.y) + ", " + std::to_string(modelPosition.z));
    Debug::log("Camera position: " + std::to_string(cameraPos.x) + ", " +
               std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z));
    Debug::log("Camera right: " + std::to_string(cameraRight.x) + ", " +
               std::to_string(cameraRight.y) + ", " + std::to_string(cameraRight.z));

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Draw the model
    Debug::log("About to draw model...");
    armModel->Draw(*armShader);
    Debug::log("Model drawn");
}