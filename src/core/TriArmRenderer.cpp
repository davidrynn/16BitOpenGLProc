#include "TriArmRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Debug.h"

void TriArmRenderer::initialize()
{
    armShader = std::make_unique<Shader>("shaders/arm.vert", "shaders/arm.frag");
    armShader->use();
    armShader->setVec3("baseColor", glm::vec3(0.4f, 0.2f, 0.6f)); // Purple-ish color for the abstract arm

    Debug::log("Starting TriArmRenderer initialization...");

    // Vertex data: position followed by normal
    float vertices[] = {
        // Forearm triangle (3 faces sharing edges) - Made larger
        -0.03f, -0.25f, -0.03f,   -0.866f, 0.0f, -0.5f,  // Left base
         0.06f, -0.25f, -0.03f,    0.866f, 0.0f, -0.5f,  // Right base
         0.015f, -0.25f, 0.03f,    0.0f, 0.0f,  1.0f,   // Back base
         0.015f, -0.08f, 0.0f,     0.0f, 1.0f,  0.0f,   // Top point

        // Palm triangle (3 faces sharing edges) - Made smaller
        -0.02f, -0.08f, -0.015f,   -0.866f, 0.0f, -0.5f,  // Left base
         0.04f, -0.08f, -0.015f,    0.866f, 0.0f, -0.5f,  // Right base
         0.01f, -0.08f,  0.015f,    0.0f, 0.0f,  1.0f,   // Back base
         0.01f,  0.04f,  0.0f,     0.0f, 1.0f,  0.0f,   // Top point

        // Thumb triangle (3 faces sharing edges)
        -0.03f, -0.08f, -0.01f,   -0.866f, 0.0f, -0.5f,  // Left base
        -0.01f, -0.04f, -0.01f,    0.866f, 0.0f, -0.5f,  // Right base
        -0.02f, -0.06f,  0.01f,    0.0f, 0.0f,  1.0f,   // Back base
        -0.05f,  0.0f,   0.0f,     0.0f, 1.0f,  0.0f,   // Top point

        // First finger triangle (3 faces sharing edges)
        -0.01f,  0.04f, -0.01f,   -0.866f, 0.0f, -0.5f,  // Left base
         0.02f,  0.04f, -0.01f,    0.866f, 0.0f, -0.5f,  // Right base
         0.005f, 0.04f,  0.01f,    0.0f, 0.0f,  1.0f,   // Back base
         0.005f, 0.09f,  0.0f,     0.0f, 1.0f,  0.0f,   // Top point

        // Second finger triangle (3 faces sharing edges)
         0.02f,  0.04f, -0.01f,   -0.866f, 0.0f, -0.5f,  // Left base
         0.05f,  0.04f, -0.01f,    0.866f, 0.0f, -0.5f,  // Right base
         0.035f, 0.04f,  0.01f,    0.0f, 0.0f,  1.0f,   // Back base
         0.035f, 0.09f,  0.0f,     0.0f, 1.0f,  0.0f    // Top point
    };

    unsigned int indices[] = {
        // Forearm (3 triangular faces)
        0, 1, 3,    // Front face
        1, 2, 3,    // Right face
        2, 0, 3,    // Left face
        
        // Palm (3 triangular faces)
        4, 5, 7,    // Front face
        5, 6, 7,    // Right face
        6, 4, 7,    // Left face

        // Thumb (3 triangular faces)
        8, 9, 11,   // Front face
        9, 10, 11,  // Right face
        10, 8, 11,  // Left face

        // First finger (3 triangular faces)
        12, 13, 15, // Front face
        13, 14, 15, // Right face
        14, 12, 15, // Left face

        // Second finger (3 triangular faces)
        16, 17, 19, // Front face
        17, 18, 19, // Right face
        18, 16, 19  // Left face
    };

    // Generate and bind buffers
    glGenVertexArrays(1, &triVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(triVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Debug::log("TriArmRenderer initialization complete.");
}

void TriArmRenderer::update(float deltaTime)
{
    const float CLENCH_THRESHOLD = 0.2f;

    if (isHolding) {
        holdTime += deltaTime;
        if (holdTime >= CLENCH_THRESHOLD) {
            clenching = true;
        }
    } else {
        holdTime = 0.0f;
        clenching = false;
    }

    // Update clenching animation
    if (clenching && clenchAmount < 1.0f) {
        clenchAmount = std::min(clenchAmount + deltaTime * 2.0f, 1.0f);
    } else if (!clenching && clenchAmount > 0.0f) {
        clenchAmount = std::max(clenchAmount - deltaTime * 2.0f, 0.0f);
    }
}

void TriArmRenderer::triggerPunch()
{
    if (!punching)
    {
        punching = true;
        punchStartTime = static_cast<float>(glfwGetTime());
    }
}

void TriArmRenderer::render(const Camera& camera, const glm::mat4& projection)
{
    armShader->use();
    armShader->setMat4("view", glm::mat4(1.0f));
    armShader->setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    // Position in bottom right, slightly forward
    model = glm::translate(model, glm::vec3(0.3f, -0.15f, -0.53f));
    
    // Apply debug rotation first, before any other rotations
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Then apply base rotations
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply punch animation
    if (punching) {
        float currentTime = static_cast<float>(glfwGetTime());
        float timeSincePunch = currentTime - punchStartTime;
        
        if (timeSincePunch < 0.2f) {
            float progress = timeSincePunch / 0.2f;
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f * progress));
        } 
        else if (timeSincePunch < 0.4f) {
            float progress = (timeSincePunch - 0.2f) / 0.2f;
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f * (1.0f - progress)));
        }
        else {
            punching = false;
        }
    }

    armShader->setMat4("model", model);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);  // Disable face culling to see all faces

    // Draw the base arm and palm
    glBindVertexArray(triVAO);
    
    // Draw forearm (3 triangular faces)
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    // Draw palm (3 triangular faces)
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)(9 * sizeof(unsigned int)));

    // Draw thumb with clenching animation
    glm::mat4 thumbModel = model;
    thumbModel = glm::translate(thumbModel, glm::vec3(-0.03f, -0.08f, 0.0f));
    float thumbClenchAngle = clenchAmount * 30.0f;
    thumbModel = glm::rotate(thumbModel, glm::radians(thumbClenchAngle), glm::vec3(0.0f, 0.0f, -1.0f));
    thumbModel = glm::translate(thumbModel, glm::vec3(0.03f, 0.08f, 0.0f));
    
    armShader->setMat4("model", thumbModel);
    // Draw thumb (3 triangular faces)
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int)));

    // Draw fingers with clenching animation
    for (int i = 0; i < 2; i++) {
        glm::mat4 fingerModel = model;
        fingerModel = glm::translate(fingerModel, glm::vec3(0.01f + i * 0.03f, 0.04f, 0.0f));
        float clenchAngle = clenchAmount * 45.0f;
        fingerModel = glm::rotate(fingerModel, glm::radians(clenchAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        fingerModel = glm::translate(fingerModel, glm::vec3(-0.01f - i * 0.03f, -0.04f, 0.0f));
        
        armShader->setMat4("model", fingerModel);
        // Draw finger (3 triangular faces each)
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)((27 + i * 9) * sizeof(unsigned int)));
    }
} 