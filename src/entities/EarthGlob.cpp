#include "EarthGlob.h"
#include <glm/gtc/matrix_transform.hpp>

void EarthGlob::initialize(const glm::vec3& startPos) {
    position = startPos;
    velocity = glm::vec3(0.0f, liftSpeed, 0.0f); // Initial upward velocity

    // Create a cube mesh for the glob with 36 vertices (6 faces * 2 triangles * 3 vertices)
    float cubeSize = size;
    float cube[] = {
        // Front face
        -cubeSize, -cubeSize, -cubeSize,  // 0
         cubeSize, -cubeSize, -cubeSize,  // 1
         cubeSize,  cubeSize, -cubeSize,  // 2
        -cubeSize,  cubeSize, -cubeSize,  // 3
        
        // Back face
        -cubeSize, -cubeSize,  cubeSize,  // 4
         cubeSize, -cubeSize,  cubeSize,  // 5
         cubeSize,  cubeSize,  cubeSize,  // 6
        -cubeSize,  cubeSize,  cubeSize,  // 7
        
        // Left face
        -cubeSize, -cubeSize,  cubeSize,  // 8
        -cubeSize, -cubeSize, -cubeSize,  // 9
        -cubeSize,  cubeSize, -cubeSize,  // 10
        -cubeSize,  cubeSize,  cubeSize,  // 11
        
        // Right face
         cubeSize, -cubeSize, -cubeSize,  // 12
         cubeSize, -cubeSize,  cubeSize,  // 13
         cubeSize,  cubeSize,  cubeSize,  // 14
         cubeSize,  cubeSize, -cubeSize,  // 15
        
        // Top face
        -cubeSize,  cubeSize, -cubeSize,  // 16
         cubeSize,  cubeSize, -cubeSize,  // 17
         cubeSize,  cubeSize,  cubeSize,  // 18
        -cubeSize,  cubeSize,  cubeSize,  // 19
        
        // Bottom face
        -cubeSize, -cubeSize, -cubeSize,  // 20
         cubeSize, -cubeSize, -cubeSize,  // 21
         cubeSize, -cubeSize,  cubeSize,  // 22
        -cubeSize, -cubeSize,  cubeSize   // 23
    };

    GLuint indices[] = {
        // Front face
        0, 1, 2,   2, 3, 0,
        // Back face
        4, 5, 6,   6, 7, 4,
        // Left face
        8, 9, 10,  10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader = std::make_unique<Shader>("shaders/debug_marker.vert", "shaders/debug_marker.frag");
}

void EarthGlob::update(float deltaTime) {
    // Apply velocity
    position += velocity * deltaTime;
    
    // Add some random movement
    float wobbleAmount = 0.1f;
    velocity.x += (rand() / (float)RAND_MAX - 0.5f) * wobbleAmount;
    velocity.z += (rand() / (float)RAND_MAX - 0.5f) * wobbleAmount;
    
    // Clamp horizontal velocity
    float maxHorizontalSpeed = 2.0f;
    glm::vec2 horizontalVel(velocity.x, velocity.z);
    if (glm::length(horizontalVel) > maxHorizontalSpeed) {
        horizontalVel = glm::normalize(horizontalVel) * maxHorizontalSpeed;
        velocity.x = horizontalVel.x;
        velocity.z = horizontalVel.y;
    }
}

void EarthGlob::render(const glm::mat4& view, const glm::mat4& projection) {
    if (!active) return;
    
    shader->use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    
    // Add some rotation based on position
    float rotationAngle = glm::radians(position.y * 45.0f);
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setVec3("color", glm::vec3(0.6f, 0.4f, 0.2f)); // Earth/dirt color

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
