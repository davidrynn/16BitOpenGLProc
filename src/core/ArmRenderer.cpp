#include "ArmRenderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Debug.h"
void ArmRenderer::initialize()
{

    armShader = std::make_unique<Shader>("shaders/arm.vert", "shaders/arm.frag");
    armShader->use();
    armShader->setVec3("baseColor", glm::vec3(0.12f, 0.02f, 0.2f));

    float vertices[] = {
        // --- Back face (Z = -0.25) ---
        -0.05f, -0.25f, -0.25f,
        0.05f, -0.25f, -0.25f,
        0.05f, 0.25f, -0.25f,
        0.05f, 0.25f, -0.25f,
        -0.05f, 0.25f, -0.25f,
        -0.05f, -0.25f, -0.25f,

        // --- Front face (Z = +0.25) ---
        -0.05f, -0.25f, 0.25f,
        0.05f, -0.25f, 0.25f,
        0.05f, 0.25f, 0.25f,
        0.05f, 0.25f, 0.25f,
        -0.05f, 0.25f, 0.25f,
        -0.05f, -0.25f, 0.25f,

        // --- Left face (X = -0.15) ---
        -0.05f, 0.25f, 0.25f,
        -0.05f, 0.25f, -0.25f,
        -0.05f, -0.25f, -0.25f,
        -0.05f, -0.25f, -0.25f,
        -0.05f, -0.25f, 0.25f,
        -0.05f, 0.25f, 0.25f,

        // --- Right face (X = +0.15) ---
        0.15f, 0.25f, 0.25f,
        0.15f, 0.25f, -0.25f,
        0.15f, -0.25f, -0.25f,
        0.15f, -0.25f, -0.25f,
        0.15f, -0.25f, 0.25f,
        0.15f, 0.25f, 0.25f,

        // --- Bottom face (Y = -0.25) ---
        -0.15f, -0.25f, -0.25f,
        0.15f, -0.25f, -0.25f,
        0.15f, -0.25f, 0.25f,
        0.15f, -0.25f, 0.25f,
        -0.15f, -0.25f, 0.25f,
        -0.15f, -0.25f, -0.25f,

        // --- Top face (Y = +0.25) ---
        -0.15f, 0.25f, -0.25f,
        0.15f, 0.25f, -0.25f,
        0.15f, 0.25f, 0

    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void ArmRenderer::triggerPunch()
{
    if (!punching)
    {
        punching = true;
        punchStartTime = static_cast<float>(glfwGetTime());
    }
}

void ArmRenderer::render(const Camera &camera, const glm::mat4 &projection)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.6f, -0.5f, -1.5f)); // View-space offset
    model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1, 0, 0));

    if (punching)
    {
        float elapsed = static_cast<float>(glfwGetTime()) - punchStartTime;
        float anim = sin(elapsed * 10.0f) * 0.3f;
        model = glm::translate(model, glm::vec3(0.0f, -fabs(anim), anim));
        if (elapsed > 0.5f)
            punching = false;
    }

    armShader->use();
    armShader->setMat4("view", glm::mat4(1.0f)); // identity to fix position
    armShader->setMat4("projection", projection);
    armShader->setMat4("model", model);

        // Enable wireframe mode if wireframeEnabled is true
    if (wireframeEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Render terrain as wireframe
    }
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    if (wireframeEnabled) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset
    }
}

