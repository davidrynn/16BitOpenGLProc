#include "ArmRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Debug.h"

void ArmRenderer::initialize()
{
    armShader = std::make_unique<Shader>("shaders/arm.vert", "shaders/arm.frag");
    armShader->use();
    armShader->setVec3("baseColor", glm::vec3(0.2f, 0.3f, 0.4f)); // Alien blue color

    Debug::log("Starting ArmRenderer initialization...");

    // Vertex data: position followed by normal
    float vertices[] = {
        // Forearm section (thinner, elongated)
        // Back face
        -0.02f, -0.25f, -0.02f,  0.0f, 0.0f, -1.0f,
         0.02f, -0.25f, -0.02f,  0.0f, 0.0f, -1.0f,
         0.02f, -0.08f, -0.02f,  0.0f, 0.0f, -1.0f,
        -0.02f, -0.08f, -0.02f,  0.0f, 0.0f, -1.0f,
        
        // Front face
        -0.02f, -0.25f,  0.02f,  0.0f, 0.0f, 1.0f,
         0.02f, -0.25f,  0.02f,  0.0f, 0.0f, 1.0f,
         0.02f, -0.08f,  0.02f,  0.0f, 0.0f, 1.0f,
        -0.02f, -0.08f,  0.02f,  0.0f, 0.0f, 1.0f,

        // Left face
        -0.02f, -0.25f, -0.02f,  -1.0f, 0.0f, 0.0f,
        -0.02f, -0.25f,  0.02f,  -1.0f, 0.0f, 0.0f,
        -0.02f, -0.08f,  0.02f,  -1.0f, 0.0f, 0.0f,
        -0.02f, -0.08f, -0.02f,  -1.0f, 0.0f, 0.0f,

        // Right face
         0.02f, -0.25f, -0.02f,   1.0f, 0.0f, 0.0f,
         0.02f, -0.25f,  0.02f,   1.0f, 0.0f, 0.0f,
         0.02f, -0.08f,  0.02f,   1.0f, 0.0f, 0.0f,
         0.02f, -0.08f, -0.02f,   1.0f, 0.0f, 0.0f,

        // Wrist joint section
        // Back face
        -0.025f, -0.08f, -0.02f,  0.0f, 0.0f, -1.0f,
         0.025f, -0.08f, -0.02f,  0.0f, 0.0f, -1.0f,
         0.025f, -0.04f, -0.02f,  0.0f, 0.0f, -1.0f,
        -0.025f, -0.04f, -0.02f,  0.0f, 0.0f, -1.0f,

        // Front face
        -0.025f, -0.08f,  0.02f,  0.0f, 0.0f, 1.0f,
         0.025f, -0.08f,  0.02f,  0.0f, 0.0f, 1.0f,
         0.025f, -0.04f,  0.02f,  0.0f, 0.0f, 1.0f,
        -0.025f, -0.04f,  0.02f,  0.0f, 0.0f, 1.0f,

        // Left face
        -0.025f, -0.08f, -0.02f,  -1.0f, 0.0f, 0.0f,
        -0.025f, -0.08f,  0.02f,  -1.0f, 0.0f, 0.0f,
        -0.025f, -0.04f,  0.02f,  -1.0f, 0.0f, 0.0f,
        -0.025f, -0.04f, -0.02f,  -1.0f, 0.0f, 0.0f,

        // Right face
         0.025f, -0.08f, -0.02f,   1.0f, 0.0f, 0.0f,
         0.025f, -0.08f,  0.02f,   1.0f, 0.0f, 0.0f,
         0.025f, -0.04f,  0.02f,   1.0f, 0.0f, 0.0f,
         0.025f, -0.04f, -0.02f,   1.0f, 0.0f, 0.0f,
        
        // Palm section (flat rectangle)
        // Back face
        -0.04f, -0.04f, -0.01f,   0.0f, 0.0f, -1.0f,
         0.04f, -0.04f, -0.01f,   0.0f, 0.0f, -1.0f,
         0.04f,  0.04f, -0.01f,   0.0f, 0.0f, -1.0f,
        -0.04f,  0.04f, -0.01f,   0.0f, 0.0f, -1.0f,
        
        // Front face
        -0.04f, -0.04f,  0.01f,   0.0f, 0.0f, 1.0f,
         0.04f, -0.04f,  0.01f,   0.0f, 0.0f, 1.0f,
         0.04f,  0.04f,  0.01f,   0.0f, 0.0f, 1.0f,
        -0.04f,  0.04f,  0.01f,   0.0f, 0.0f, 1.0f,

        // Left face
        -0.04f, -0.04f, -0.01f,   -1.0f, 0.0f, 0.0f,
        -0.04f, -0.04f,  0.01f,   -1.0f, 0.0f, 0.0f,
        -0.04f,  0.04f,  0.01f,   -1.0f, 0.0f, 0.0f,
        -0.04f,  0.04f, -0.01f,   -1.0f, 0.0f, 0.0f,

        // Right face
         0.04f, -0.04f, -0.01f,    1.0f, 0.0f, 0.0f,
         0.04f, -0.04f,  0.01f,    1.0f, 0.0f, 0.0f,
         0.04f,  0.04f,  0.01f,    1.0f, 0.0f, 0.0f,
         0.04f,  0.04f, -0.01f,    1.0f, 0.0f, 0.0f,

        // Top face
        -0.04f,  0.04f, -0.01f,    0.0f, 1.0f, 0.0f,
         0.04f,  0.04f, -0.01f,    0.0f, 1.0f, 0.0f,
         0.04f,  0.04f,  0.01f,    0.0f, 1.0f, 0.0f,
        -0.04f,  0.04f,  0.01f,    0.0f, 1.0f, 0.0f,

        // Bottom face
        -0.04f, -0.04f, -0.01f,    0.0f, -1.0f, 0.0f,
         0.04f, -0.04f, -0.01f,    0.0f, -1.0f, 0.0f,
         0.04f, -0.04f,  0.01f,    0.0f, -1.0f, 0.0f,
        -0.04f, -0.04f,  0.01f,    0.0f, -1.0f, 0.0f,

        // Three alien fingers (extending from palm)
        // Finger 1 (left)
        // Front face
        -0.035f, 0.04f, -0.005f,   0.0f, 0.5f, 0.5f,  // Base left
        -0.025f, 0.04f, -0.005f,   0.0f, 0.5f, 0.5f,  // Base right
        -0.02f,  0.10f, -0.04f,    0.0f, 0.5f, 0.5f,  // Tip right
        -0.03f,  0.10f, -0.04f,    0.0f, 0.5f, 0.5f,  // Tip left

        // Back face
        -0.035f, 0.04f,  0.005f,   0.0f, 0.5f, -0.5f,  // Base left
        -0.025f, 0.04f,  0.005f,   0.0f, 0.5f, -0.5f,  // Base right
        -0.02f,  0.10f, -0.03f,    0.0f, 0.5f, -0.5f,  // Tip right
        -0.03f,  0.10f, -0.03f,    0.0f, 0.5f, -0.5f,  // Tip left

        // Side faces
        -0.035f, 0.04f, -0.005f,   -1.0f, 0.5f, 0.0f,  // Left side
        -0.03f,  0.10f, -0.04f,    -1.0f, 0.5f, 0.0f,
        -0.03f,  0.10f, -0.03f,    -1.0f, 0.5f, 0.0f,
        -0.035f, 0.04f,  0.005f,   -1.0f, 0.5f, 0.0f,

        -0.025f, 0.04f, -0.005f,    1.0f, 0.5f, 0.0f,  // Right side
        -0.02f,  0.10f, -0.04f,     1.0f, 0.5f, 0.0f,
        -0.02f,  0.10f, -0.03f,     1.0f, 0.5f, 0.0f,
        -0.025f, 0.04f,  0.005f,    1.0f, 0.5f, 0.0f,
        
        // Finger 2 (middle)
        // Front face
        -0.005f, 0.04f, -0.005f,   0.0f, 0.5f, 0.5f,  // Base left
         0.005f, 0.04f, -0.005f,   0.0f, 0.5f, 0.5f,  // Base right
         0.008f, 0.12f, -0.05f,    0.0f, 0.5f, 0.5f,  // Tip right
        -0.008f, 0.12f, -0.05f,    0.0f, 0.5f, 0.5f,  // Tip left

        // Back face
        -0.005f, 0.04f,  0.005f,   0.0f, 0.5f, -0.5f,  // Base left
         0.005f, 0.04f,  0.005f,   0.0f, 0.5f, -0.5f,  // Base right
         0.008f, 0.12f, -0.04f,    0.0f, 0.5f, -0.5f,  // Tip right
        -0.008f, 0.12f, -0.04f,    0.0f, 0.5f, -0.5f,  // Tip left

        // Side faces
        -0.005f, 0.04f, -0.005f,   -1.0f, 0.5f, 0.0f,  // Left side
        -0.008f, 0.12f, -0.05f,    -1.0f, 0.5f, 0.0f,
        -0.008f, 0.12f, -0.04f,    -1.0f, 0.5f, 0.0f,
        -0.005f, 0.04f,  0.005f,   -1.0f, 0.5f, 0.0f,

         0.005f, 0.04f, -0.005f,    1.0f, 0.5f, 0.0f,  // Right side
         0.008f, 0.12f, -0.05f,     1.0f, 0.5f, 0.0f,
         0.008f, 0.12f, -0.04f,     1.0f, 0.5f, 0.0f,
         0.005f, 0.04f,  0.005f,    1.0f, 0.5f, 0.0f,
        
        // Finger 3 (right)
        // Front face
         0.025f, 0.04f, -0.005f,   0.0f, 0.5f, 0.5f,  // Base left
         0.035f, 0.04f, -0.005f,   0.0f, 0.5f, 0.5f,  // Base right
         0.03f,  0.10f, -0.04f,    0.0f, 0.5f, 0.5f,  // Tip right
         0.02f,  0.10f, -0.04f,    0.0f, 0.5f, 0.5f,  // Tip left

        // Back face
         0.025f, 0.04f,  0.005f,   0.0f, 0.5f, -0.5f,  // Base left
         0.035f, 0.04f,  0.005f,   0.0f, 0.5f, -0.5f,  // Base right
         0.03f,  0.10f, -0.03f,    0.0f, 0.5f, -0.5f,  // Tip right
         0.02f,  0.10f, -0.03f,    0.0f, 0.5f, -0.5f,  // Tip left

        // Side faces
         0.025f, 0.04f, -0.005f,   -1.0f, 0.5f, 0.0f,  // Left side
         0.02f,  0.10f, -0.04f,    -1.0f, 0.5f, 0.0f,
         0.02f,  0.10f, -0.03f,    -1.0f, 0.5f, 0.0f,
         0.025f, 0.04f,  0.005f,   -1.0f, 0.5f, 0.0f,

         0.035f, 0.04f, -0.005f,    1.0f, 0.5f, 0.0f,  // Right side
         0.03f,  0.10f, -0.04f,     1.0f, 0.5f, 0.0f,
         0.03f,  0.10f, -0.03f,     1.0f, 0.5f, 0.0f,
         0.035f, 0.04f,  0.005f,    1.0f, 0.5f, 0.0f,

        // Thumb (angled from palm's side)
        // Base section - Front face
        -0.04f,  0.0f,  -0.005f,   -0.5f, 0.5f, 0.5f,  // Base left
        -0.04f,  0.01f, -0.005f,   -0.5f, 0.5f, 0.5f,  // Base right
        -0.06f,  0.03f, -0.02f,    -0.5f, 0.5f, 0.5f,  // Tip right
        -0.07f,  0.02f, -0.02f,    -0.5f, 0.5f, 0.5f,  // Tip left

        // Base section - Back face
        -0.04f,  0.0f,   0.005f,   -0.5f, 0.5f, -0.5f,  // Base left
        -0.04f,  0.01f,  0.005f,   -0.5f, 0.5f, -0.5f,  // Base right
        -0.06f,  0.03f, -0.01f,    -0.5f, 0.5f, -0.5f,  // Tip right
        -0.07f,  0.02f, -0.01f,    -0.5f, 0.5f, -0.5f,  // Tip left

        // Base section - Top face
        -0.04f,  0.01f, -0.005f,   -0.5f, 1.0f, 0.0f,  // Base front
        -0.04f,  0.01f,  0.005f,   -0.5f, 1.0f, 0.0f,  // Base back
        -0.06f,  0.03f, -0.01f,    -0.5f, 1.0f, 0.0f,  // Tip back
        -0.06f,  0.03f, -0.02f,    -0.5f, 1.0f, 0.0f,  // Tip front

        // Base section - Bottom face
        -0.04f,  0.0f,  -0.005f,   -0.5f, -1.0f, 0.0f,  // Base front
        -0.04f,  0.0f,   0.005f,   -0.5f, -1.0f, 0.0f,  // Base back
        -0.07f,  0.02f, -0.01f,    -0.5f, -1.0f, 0.0f,  // Tip back
        -0.07f,  0.02f, -0.02f,    -0.5f, -1.0f, 0.0f,  // Tip front

        // Tip section - Front face
        -0.06f,  0.03f, -0.02f,    -0.5f, 0.5f, 0.5f,  // Base left
        -0.07f,  0.02f, -0.02f,    -0.5f, 0.5f, 0.5f,  // Base right
        -0.07f,  0.04f, -0.035f,   -0.5f, 0.5f, 0.5f,  // Tip right
        -0.08f,  0.03f, -0.035f,   -0.5f, 0.5f, 0.5f,  // Tip left

        // Tip section - Back face
        -0.06f,  0.03f, -0.01f,    -0.5f, 0.5f, -0.5f,  // Base left
        -0.07f,  0.02f, -0.01f,    -0.5f, 0.5f, -0.5f,  // Base right
        -0.07f,  0.04f, -0.025f,   -0.5f, 0.5f, -0.5f,  // Tip right
        -0.08f,  0.03f, -0.025f,   -0.5f, 0.5f, -0.5f,  // Tip left

        // Tip section - Top face
        -0.06f,  0.03f, -0.02f,    -0.5f, 1.0f, 0.0f,  // Base front
        -0.06f,  0.03f, -0.01f,    -0.5f, 1.0f, 0.0f,  // Base back
        -0.07f,  0.04f, -0.025f,   -0.5f, 1.0f, 0.0f,  // Tip back
        -0.07f,  0.04f, -0.035f,   -0.5f, 1.0f, 0.0f   // Tip front
    };

    unsigned int indices[] = {
        // Forearm
        0, 1, 2,    2, 3, 0,     // Back face
        4, 5, 6,    6, 7, 4,     // Front face
        8, 9, 10,   10, 11, 8,   // Left face
        12, 13, 14, 14, 15, 12,  // Right face

        // Wrist joint
        16, 17, 18, 18, 19, 16,  // Back face
        20, 21, 22, 22, 23, 20,  // Front face
        24, 25, 26, 26, 27, 24,  // Left face
        28, 29, 30, 30, 31, 28,  // Right face

        // Palm
        32, 33, 34, 34, 35, 32,  // Back face
        36, 37, 38, 38, 39, 36,  // Front face
        40, 41, 42, 42, 43, 40,  // Left face
        44, 45, 46, 46, 47, 44,  // Right face
        48, 49, 50, 50, 51, 48,  // Top face
        52, 53, 54, 54, 55, 52,  // Bottom face

        // Finger 1 (left)
        56, 57, 58, 58, 59, 56,  // Front face
        60, 61, 62, 62, 63, 60,  // Back face
        64, 65, 66, 66, 67, 64,  // Left side
        68, 69, 70, 70, 71, 68,  // Right side

        // Finger 2 (middle)
        72, 73, 74, 74, 75, 72,  // Front face
        76, 77, 78, 78, 79, 76,  // Back face
        80, 81, 82, 82, 83, 80,  // Left side
        84, 85, 86, 86, 87, 84,  // Right side

        // Finger 3 (right)
        88, 89, 90, 90, 91, 88,  // Front face
        92, 93, 94, 94, 95, 92,  // Back face
        96, 97, 98, 98, 99, 96,  // Left side
        100, 101, 102, 102, 103, 100,  // Right side

        // Thumb base section
        104, 105, 106, 106, 107, 104,  // Front face
        108, 109, 110, 110, 111, 108,  // Back face
        112, 113, 114, 114, 115, 112,  // Top face
        116, 117, 118, 118, 119, 116,  // Bottom face

        // Thumb tip section
        120, 121, 122, 122, 123, 120,  // Front face
        124, 125, 126, 126, 127, 124,  // Back face
        128, 129, 130, 130, 131, 128,  // Top face
        132, 133, 134, 134, 135, 132   // Bottom face
    };

    // Clean up any existing buffers
    if (cubeVAO != 0) {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    // Generate new buffers
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind and set up vertex array object
    glBindVertexArray(cubeVAO);

    // Copy vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Copy index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set up vertex attributes
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Debug::log("ArmRenderer initialization complete.");
}

void ArmRenderer::triggerPunch()
{
    if (!punching)
    {
        punching = true;
        punchStartTime = static_cast<float>(glfwGetTime());
    }
}

void ArmRenderer::update(float deltaTime)
{
    const float CLENCH_THRESHOLD = 0.2f;  // Time to hold before clenching starts

    if (isHolding) {
        holdTime += deltaTime;
        if (holdTime >= CLENCH_THRESHOLD) {
            clenching = true;  // Start/maintain clenching after threshold
        }
    } else {
        holdTime = 0.0f;
        clenching = false;  // Stop clenching when released
    }

    // Update clenching animation
    if (clenching && clenchAmount < 1.0f) {
        clenchAmount = std::min(clenchAmount + deltaTime * 2.0f, 1.0f);  // Smooth transition over 0.5 seconds
    } else if (!clenching && clenchAmount > 0.0f) {
        clenchAmount = std::max(clenchAmount - deltaTime * 2.0f, 0.0f);  // Smooth transition back
    }
}

void ArmRenderer::render(const Camera& camera, const glm::mat4& projection)
{
    armShader->use();
    armShader->setMat4("view", glm::mat4(1.0f));  // Identity matrix for view since we want it in view space
    armShader->setMat4("projection", projection);

    // Create and set model matrix
    glm::mat4 model = glm::mat4(1.0f);
    // Position in center of screen, slightly forward
    model = glm::translate(model, glm::vec3(0.3f, -0.15f, -0.53f));
    
    // Apply debug rotation from mouse wheel around the arm's length (y-axis)
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Base rotation to angle the arm slightly
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Slight lean
    model = glm::rotate(model, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Reach hand down on x axis.
    model = glm::rotate(model, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate palm up on y axis.

    // Apply punching animation if active
    if (punching) {
        float currentTime = static_cast<float>(glfwGetTime());
        float timeSincePunch = currentTime - punchStartTime;
        
        if (timeSincePunch < 0.2f) {  // Forward punch
            float progress = timeSincePunch / 0.2f;
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f * progress));
        } 
        else if (timeSincePunch < 0.4f) {  // Retract
            float progress = (timeSincePunch - 0.2f) / 0.2f;
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f * (1.0f - progress)));
        }
        else {
            punching = false;  // Animation complete
        }
    }

    armShader->setMat4("model", model);

    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);  // Disable face culling for debugging

    // Draw the palm and forearm
    glBindVertexArray(cubeVAO);
    
    // Draw base arm parts (forearm, wrist, palm)
    glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);  // 24 triangles * 3 vertices for forearm and wrist
    
    // Draw palm
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(72 * sizeof(unsigned int)));

    // Draw fingers with clenching animation
    for (int finger = 0; finger < 3; finger++) {
        glm::mat4 fingerModel = model;
        // Move to finger base position
        fingerModel = glm::translate(fingerModel, glm::vec3(-0.035f + finger * 0.03f, 0.04f, 0.0f));
        // Apply clenching rotation
        float clenchAngle = clenchAmount * 45.0f;  // Rotate up to 45 degrees
        fingerModel = glm::rotate(fingerModel, glm::radians(clenchAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        // Move back to original position
        fingerModel = glm::translate(fingerModel, glm::vec3(0.035f - finger * 0.03f, -0.04f, 0.0f));
        
        armShader->setMat4("model", fingerModel);
        // Draw finger (24 vertices per finger)
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)((108 + finger * 24) * sizeof(unsigned int)));
    }

    // Draw thumb with clenching animation
    glm::mat4 thumbModel = model;
    // Move to thumb base position
    thumbModel = glm::translate(thumbModel, glm::vec3(-0.04f, 0.0f, 0.0f));
    // Apply clenching rotation
    float thumbClenchAngle = clenchAmount * 60.0f;  // Rotate up to 60 degrees
    thumbModel = glm::rotate(thumbModel, glm::radians(thumbClenchAngle), glm::vec3(0.0f, 0.0f, -1.0f));
    // Move back to original position
    thumbModel = glm::translate(thumbModel, glm::vec3(0.04f, 0.0f, 0.0f));
    
    armShader->setMat4("model", thumbModel);
    // Draw thumb base and tip
    glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(180 * sizeof(unsigned int)));
}

