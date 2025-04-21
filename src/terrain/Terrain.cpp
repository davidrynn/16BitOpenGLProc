#include "Terrain.h"
#include <cmath>
#include <iostream>
#include "BiomeManager.h"
#include "ChunkConstants.h"
#include "TerrainConstants.h"
#include "TerrainNoiseFactory.h"

static inline BiomeManager biomeManager;

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

float Terrain::getHeightAt(float worldX, float worldZ)
{
    TerrainType terrainType = getTerrainTypeAt(worldX, worldZ);
    auto noise = TerrainNoiseFactory::getNoise(terrainType);
    return noise ? noise->getHeight(worldX, worldZ) : 0.0f;
}

void Terrain::initialize(std::function<void(float)> progressCallback)
{
    biomeManager.initialize(
        TerrainConstants::DEFAULT_BIOME_COUNT,
        TerrainConstants::BIOME_WORLD_WIDTH);
    const int numOfChunks = TerrainConstants::INITIAL_CHUNK_RADIUS;
    const int totalSteps = (2 * numOfChunks + 1) * (2 * numOfChunks + 1);
    int currentStep = 0;
    for (int z = -numOfChunks; z <= numOfChunks; ++z)
    {
        for (int x = -numOfChunks; x <= numOfChunks; ++x)
        {
            chunks[{x, z}] = new Chunk(x, z);
            if (progressCallback) {
                float progress = static_cast<float>(++currentStep) / totalSteps;
                progressCallback(progress);
            }
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



// void Terrain::incrementalInitializeStep(int step, int totalSteps)
// {
//     // For now, just do full init once.
//     // Later, divide this into true staged loading.
//     if (step == totalSteps)
//         initialize();
// }

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

