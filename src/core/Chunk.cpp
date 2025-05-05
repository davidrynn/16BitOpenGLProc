#include "Chunk.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ChunkConstants.h"
#include "Debug.h"
#include "Shader.h" // Include Shader to set uniforms
#include "Terrain.h"

static constexpr int SIZE = ChunkConstants::SIZE;

Chunk::Chunk(int x, int z, std::shared_ptr<Terrain> terrain, bool renderingEnabled) : chunkX(x), chunkZ(z), terrain(std::move(terrain)), spacing(1.0f)
{
    generate();
    if (renderingEnabled) uploadToGPU();
}

Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Chunk::generate()
{
    vertices.clear();
    indices.clear();
    normals.clear();      // Add a container for normals

    // Generate vertices and heights
    for (int z = 0; z <= SIZE; ++z)
    {
        for (int x = 0; x <= SIZE; ++x)
        {
            float localX = x * spacing;
            float localZ = z * spacing;
            float worldX = chunkX * SIZE * spacing + localX;
            float worldZ = chunkZ * SIZE * spacing + localZ;

            float height = terrain->getHeightAt(worldX, worldZ);

            vertices.push_back(localX);
            vertices.push_back(height);
            vertices.push_back(localZ);

            // Initialize normals to zero (to accumulate them later)
            normals.push_back(0.0f);
            normals.push_back(0.0f);
            normals.push_back(0.0f);
        }
    }

    // Generate indices and calculate normals
    for (int z = 0; z < SIZE; ++z)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            int start = z * (SIZE + 1) + x;

            // Triangle 1
            indices.push_back(start);
            indices.push_back(start + 1);
            indices.push_back(start + SIZE + 1);

            // Triangle 2
            indices.push_back(start + 1);
            indices.push_back(start + SIZE + 2);
            indices.push_back(start + SIZE + 1);

            // Calculate normals for both triangles
            glm::vec3 v0(vertices[start * 3], vertices[start * 3 + 1], vertices[start * 3 + 2]);
            glm::vec3 v1(vertices[(start + 1) * 3], vertices[(start + 1) * 3 + 1], vertices[(start + 1) * 3 + 2]);
            glm::vec3 v2(vertices[(start + SIZE + 1) * 3], vertices[(start + SIZE + 1) * 3 + 1], vertices[(start + SIZE + 1) * 3 + 2]);
            glm::vec3 v3(vertices[(start + SIZE + 2) * 3], vertices[(start + SIZE + 2) * 3 + 1], vertices[(start + SIZE + 2) * 3 + 2]);

            // Normalize each triangle's normal before accumulation
            glm::vec3 normal1 = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            glm::vec3 normal2 = glm::normalize(glm::cross(v3 - v1, v2 - v1));

            // Accumulate normals for each vertex
            for (int i = 0; i < 3; ++i)
            {
                normals[start * 3 + i] += normal1[i];
                normals[(start + 1) * 3 + i] += normal1[i];
                normals[(start + SIZE + 1) * 3 + i] += normal1[i];

                normals[(start + 1) * 3 + i] += normal2[i];
                normals[(start + SIZE + 2) * 3 + i] += normal2[i];
                normals[(start + SIZE + 1) * 3 + i] += normal2[i];
            }
        }
    }

    // Normalize all accumulated normals
    for (size_t i = 0; i < normals.size(); i += 3)
    {
        glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
        normal = glm::normalize(normal);
        normals[i] = normal.x;
        normals[i + 1] = normal.y;
        normals[i + 2] = normal.z;
    }
    // // Debug: Print normals
    // for (size_t i = 0; i < normals.size(); i += 3) {
    //     glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
    //     float length = glm::length(normal);
    //     std::cout << "Normal " << i / 3 << ": ("
    //               << normals[i] << ", "
    //               << normals[i + 1] << ", "
    //               << normals[i + 2] << "), Length: " << length << std::endl;
    // }
}

void Chunk::uploadToGPU()
{
    if (!renderingEnabled) return;
    std::vector<float> vertexData;
    for (size_t i = 0; i < vertices.size() / 3; ++i)
    {
        vertexData.push_back(vertices[i * 3]);     // x
        vertexData.push_back(vertices[i * 3 + 1]); // y
        vertexData.push_back(vertices[i * 3 + 2]); // z
        vertexData.push_back(normals[i * 3]);      // nx
        vertexData.push_back(normals[i * 3 + 1]);  // ny
        vertexData.push_back(normals[i * 3 + 2]);  // nz
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    uploaded = true;

}

void Chunk::render(Shader &shader) const
{
    if (!renderingEnabled) return;
    // Apply translation based on chunk world coordinates
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunkX * SIZE, 0.0f, chunkZ * SIZE));
    shader.setMat4("model", model);

    // Enable wireframe mode if wireframeEnabled is true
    if (wireframeEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Render terrain as wireframe
    }

    // Regular terrain rendering
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

    // Reset to fill mode after rendering
    if (wireframeEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to normal rendering mode
    }

    // Optional debug: draw bounding box
    if (wireframeEnabled)
    {
        drawChunkBoundingBox();
    }
}

void Chunk::drawChunkBoundingBox() const
{
    float yMin = -1.0f, yMax = 10.0f;
    float minX = chunkX * SIZE * spacing; // Incorporate spacing
    float minZ = chunkZ * SIZE * spacing; // Incorporate spacing
    float maxX = minX + SIZE * spacing;
    float maxZ = minZ + SIZE * spacing;    

    float boxVertices[] = {
        minX, yMin, minZ, maxX, yMin, minZ,
        maxX, yMin, minZ, maxX, yMin, maxZ,
        maxX, yMin, maxZ, minX, yMin, maxZ,
        minX, yMin, maxZ, minX, yMin, minZ,

        minX, yMax, minZ, maxX, yMax, minZ,
        maxX, yMax, minZ, maxX, yMax, maxZ,
        maxX, yMax, maxZ, minX, yMax, maxZ,
        minX, yMax, maxZ, minX, yMax, minZ,

        minX, yMin, minZ, minX, yMax, minZ,
        maxX, yMin, minZ, maxX, yMax, minZ,
        maxX, yMin, maxZ, maxX, yMax, maxZ,
        minX, yMin, maxZ, minX, yMax, maxZ,
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
    glDisable(GL_DEPTH_TEST); // So lines don't clip through geometry

    glDrawArrays(GL_LINES, 0, 24);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDeleteBuffers(1, &boxVBO);
    glDeleteVertexArrays(1, &boxVAO);
}