#include "Terrain.h"
#include <cmath>
#include <iostream>
#include "BiomeManager.h"
#include "ChunkConstants.h"
#include "TerrainConstants.h"
#include "TerrainNoiseFactory.h"

static inline BiomeManager biomeManager;

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

std::vector<unsigned int> Terrain::generateIndices(int gridSize)
{
    std::vector<unsigned int> indices;

    for (int z = 0; z < gridSize - 1; z++)
    {
        for (int x = 0; x < gridSize - 1; x++)
        {
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

TerrainType Terrain::getTerrainTypeAt(float worldX, float worldZ)
{
    Biome biome = biomeManager.getBiomeForPosition(worldX, worldZ);
    return biome.getDominantTerrain();
}

// TerrainType Terrain::getTerrainTypeAt(int worldX, int worldZ) {
//     // Example biome pattern based on world coordinates
//     int pattern = (worldX / 50 + worldZ / 50) % 4;

//     switch (pattern) {
//         case 0: return TerrainType::Plains;
//         case 1: return TerrainType::Mountains;
//         case 2: return TerrainType::Desert;
//         case 3: return TerrainType::Snow;
//         default: return TerrainType::Plains;
//     }
// }

float Terrain::getHeightAt(float worldX, float worldZ)
{
    TerrainType terrainType = getTerrainTypeAt(worldX, worldZ);
    auto noise = TerrainNoiseFactory::getNoise(terrainType);
    return noise ? noise->getHeight(worldX, worldZ) : 0.0f;
}

void Terrain::initialize()
{
    biomeManager.initialize(
        TerrainConstants::DEFAULT_BIOME_COUNT,
        TerrainConstants::BIOME_WORLD_WIDTH);
    const int numOfChunks = TerrainConstants::INITIAL_CHUNK_RADIUS;
    for (int z = -numOfChunks; z <= numOfChunks; ++z)
    {
        for (int x = -numOfChunks; x <= numOfChunks; ++x)
        {
            chunks[{x, z}] = new Chunk(x, z);
        }
    }
}

void Terrain::render(Shader &shader, float playerX, float playerZ)
{
    const float renderDistance = 50.0f; // Adjust as needed

    for (auto &pair : chunks)
    {
        int chunkX = pair.first.first;
        int chunkZ = pair.first.second;

        float worldX = (chunkX + 0.5f) * ChunkConstants::SIZE; // Using 0.5f to set World position in middle of chunk.
        float worldZ = (chunkZ + 0.5f) * ChunkConstants::SIZE;


        float dx = worldX - playerX;
        float dz = worldZ - playerZ;
        float distanceSquared = dx * dx + dz * dz;

        if (distanceSquared < renderDistance * renderDistance)
        {
            pair.second->render(shader);
        }
    }
}

void Terrain::generateChunkData(int chunkX, int chunkZ,
                                std::vector<float> &outVertices,
                                std::vector<unsigned int> &outIndices,
                                std::vector<float> &outNormals)
{
    const int CHUNK_SIZE = ChunkConstants::SIZE;

    for (int z = 0; z <= CHUNK_SIZE; ++z)
    {
        for (int x = 0; x <= CHUNK_SIZE; ++x)
        {
            float worldX = static_cast<float>(chunkX * CHUNK_SIZE + x);
            float worldZ = static_cast<float>(chunkZ * CHUNK_SIZE + z);

            // Get terrain type from biome manager
            TerrainType type = biomeManager.getTerrainType(worldX, worldZ);
            const auto *noise = TerrainNoiseFactory::getNoise(type);

            float height = noise ? noise->getHeight(worldX, worldZ) : 0.0f;

            outVertices.push_back(worldX);
            outVertices.push_back(height);
            outVertices.push_back(worldZ);

            // Basic upward normal
            outNormals.push_back(0.0f);
            outNormals.push_back(1.0f);
            outNormals.push_back(0.0f);
        }
    }

    // Generate indices
    for (int z = 0; z < CHUNK_SIZE; ++z)
    {
        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            int topLeft = z * (CHUNK_SIZE + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (CHUNK_SIZE + 1) + x;
            int bottomRight = bottomLeft + 1;

            outIndices.push_back(topLeft);
            outIndices.push_back(bottomLeft);
            outIndices.push_back(topRight);

            outIndices.push_back(topRight);
            outIndices.push_back(bottomLeft);
            outIndices.push_back(bottomRight);
        }
    }
}

void Terrain::updateChunks(float playerX, float playerZ)
{
    int playerChunkX = static_cast<int>(floor(playerX / ChunkConstants::SIZE));
    int playerChunkZ = static_cast<int>(floor(playerZ / ChunkConstants::SIZE));

    const int viewDistance = 2;  // Adjust based on desired chunk visibility

    // Load chunks explicitly
    for (int z = -viewDistance; z <= viewDistance; ++z)
    {
        for (int x = -viewDistance; x <= viewDistance; ++x)
        {
            int chunkX = playerChunkX + x;
            int chunkZ = playerChunkZ + z;

            if (chunks.find({chunkX, chunkZ}) == chunks.end())
                chunks[{chunkX, chunkZ}] = new Chunk(chunkX, chunkZ);
        }
    }

    // Unload chunks explicitly
    for (auto it = chunks.begin(); it != chunks.end();)
    {
        int chunkX = it->first.first;
        int chunkZ = it->first.second;

        if (abs(chunkX - playerChunkX) > viewDistance ||
            abs(chunkZ - playerChunkZ) > viewDistance)
        {
            delete it->second;
            it = chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

