#include "Chunk.h"
#include <cmath>
#include <iostream>

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

    for (int z = 0; z <= SIZE; ++z) {
        for (int x = 0; x <= SIZE; ++x) {
            float worldX = chunkX * SIZE + x;
            float worldZ = chunkZ * SIZE + z;
            float height = std::sin(worldX * 0.1f) * std::cos(worldZ * 0.1f) * 2.0f;

            vertices.push_back(worldX);
            vertices.push_back(height);
            vertices.push_back(worldZ);
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

void Chunk::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

glm::vec2 Chunk::getChunkPos() const {
    return glm::vec2(chunkX, chunkZ);
}