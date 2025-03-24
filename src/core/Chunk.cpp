#include "Chunk.h"
#include "Debug.h"
#include "Shader.h" // Include Shader to set uniforms
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern bool wireframeEnabled; // Declare the external variable

Chunk::Chunk(int x, int z) : chunkX(x), chunkZ(z) {
    generate();
    setupBuffers();
}

Chunk::~Chunk() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Chunk::generate() {
    vertices.clear();
    indices.clear();
    float spacing = 1.0f; // Adjust this value to control the density of the terrain

    for (int z = 0; z <= SIZE; ++z) {
        for (int x = 0; x <= SIZE; ++x) {
            float localX = x * spacing;
            float localZ = z * spacing;
            float worldX = chunkX * SIZE * spacing + localX;
            float worldZ = chunkZ * SIZE * spacing + localZ;
            
            float height = std::sin(worldX * 0.1f) * std::cos(worldZ * 0.1f) * 2.0f;

            vertices.push_back(localX);
            vertices.push_back(height);
            vertices.push_back(localZ);
        }
    }

    for (int z = 0; z < SIZE; ++z) {
        for (int x = 0; x < SIZE; ++x) {
            int start = z * (SIZE + 1) + x;
            indices.push_back(start);
            indices.push_back(start + 1);
            indices.push_back(start + SIZE + 1);

            indices.push_back(start + 1);
            indices.push_back(start + SIZE + 2);
            indices.push_back(start + SIZE + 1);
        }
    }
}

void Chunk::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Chunk::render(Shader& shader) {
    // Apply translation based on chunk world coordinates
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunkX * SIZE, 0.0f, chunkZ * SIZE));
    shader.setMat4("model", model);

    // Regular terrain rendering
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Optional debug: draw bounding box
    if (wireframeEnabled) {
        drawChunkBoundingBox();
    }
}

void Chunk::drawChunkBoundingBox() {
    float yMin = -1.0f, yMax = 10.0f;
    float minX = chunkX * SIZE;
    float minZ = chunkZ * SIZE;
    float maxX = minX + SIZE;
    float maxZ = minZ + SIZE;

    float boxVertices[] = {
        minX, yMin, minZ,  maxX, yMin, minZ,
        maxX, yMin, minZ,  maxX, yMin, maxZ,
        maxX, yMin, maxZ,  minX, yMin, maxZ,
        minX, yMin, maxZ,  minX, yMin, minZ,

        minX, yMax, minZ,  maxX, yMax, minZ,
        maxX, yMax, minZ,  maxX, yMax, maxZ,
        maxX, yMax, maxZ,  minX, yMax, maxZ,
        minX, yMax, maxZ,  minX, yMax, minZ,

        minX, yMin, minZ,  minX, yMax, minZ,
        maxX, yMin, minZ,  maxX, yMax, minZ,
        maxX, yMin, maxZ,  maxX, yMax, maxZ,
        minX, yMin, maxZ,  minX, yMax, maxZ,
    };

    GLuint boxVAO, boxVBO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);

    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Set line mode + color
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_DEPTH_TEST);  // So lines don't clip through geometry

    glDrawArrays(GL_LINES, 0, 24);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDeleteBuffers(1, &boxVBO);
    glDeleteVertexArrays(1, &boxVAO);
}

glm::vec2 Chunk::getChunkPos() const {
    return glm::vec2(chunkX, chunkZ);
}