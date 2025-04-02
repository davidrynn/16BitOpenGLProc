#include "Terrain.h"
#include <cmath>
#include <iostream>
#include "TerrainNoiseFactory.h"

// std::vector<float> Terrain::generateTerrain(int gridSize, int chunkX, int chunkZ) {
//     std::vector<float> vertices;
//     float scale = 2.0f / (gridSize - 1);  // Normalize to range -1 to 1
//     float halfGrid = gridSize / 2.0f;

//     for (int z = 0; z < gridSize; ++z) {
//         for (int x = 0; x < gridSize; ++x) {
//             // Absolute world position
//             int worldX = x + chunkX * gridSize;
//             int worldZ = z + chunkZ * gridSize;

//             float xpos = x * scale - halfGrid;
//             float zpos = z * scale - halfGrid;

//             TerrainType terrainType = getTerrainTypeAt(worldX, worldZ);
//             auto noise = TerrainNoiseFactory::createNoiseFunction(terrainType);

//             float ypos = noise ? noise->getHeight(worldX, worldZ) : 0.0f;

//             vertices.push_back(xpos);
//             vertices.push_back(ypos);
//             vertices.push_back(zpos);
//         }
//     }
//     return vertices;
// }

std::vector<unsigned int> Terrain::generateIndices(int gridSize) {
    std::vector<unsigned int> indices;

    for (int z = 0; z < gridSize - 1; z++) {
        for (int x = 0; x < gridSize - 1; x++) {
            int topLeft = z * gridSize + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * gridSize + x;
            int bottomRight = bottomLeft + 1;

            // Two triangles per grid square
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    return indices;
}

TerrainType Terrain::getTerrainTypeAt(int worldX, int worldZ) {
    // Example biome pattern based on world coordinates
    int pattern = (worldX / 50 + worldZ / 50) % 4;

    switch (pattern) {
        case 0: return TerrainType::Plains;
        case 1: return TerrainType::Mountains;
        case 2: return TerrainType::Desert;
        case 3: return TerrainType::Snow;
        default: return TerrainType::Plains;
    }
}

float Terrain::getHeightAt(float worldX, float worldZ) {
    TerrainType terrainType = getTerrainTypeAt(worldX, worldZ);
    auto noise = TerrainNoiseFactory::createNoiseFunction(terrainType);
    return noise ? noise->getHeight(worldX, worldZ) : 0.0f;
}

void Terrain::initialize() {
    const int numOfChunks = 5; 
    for (int z = -numOfChunks; z <= numOfChunks; ++z) {
        for (int x = -numOfChunks; x <= numOfChunks; ++x) {
            chunks[{x, z}] = new Chunk(x, z);
        }
    }
}

void Terrain::render(Shader& shader, float playerX, float playerZ) {
    const float renderDistance = 50.0f;  // Adjust as needed

    for (auto& pair : chunks) {
        int chunkX = pair.first.first;
        int chunkZ = pair.first.second;

        float worldX = chunkX * Chunk::SIZE;
        float worldZ = chunkZ * Chunk::SIZE;

        float dx = worldX - playerX;
        float dz = worldZ - playerZ;
        float distanceSquared = dx * dx + dz * dz;

        if (distanceSquared < renderDistance * renderDistance) {
            pair.second->render(shader);
        }
    }
}

