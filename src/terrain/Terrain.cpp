#include "Terrain.h"
#include <cmath>
#include <iostream>
#include "BiomeManager.h"
#include "ChunkConstants.h"
#include "DefaultChunkFactory.h"
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
    assert(noiseFactory && "TerrainNoiseFactory is null!");
    auto noiseFn = noiseFactory->getNoise(terrainType);
    return noiseFn ? noiseFn(worldX, worldZ) : 0.0f;
}
//    auto noiseFn = noiseFactory.getNoise(terrainType);


void Terrain::setChunkFactory(std::shared_ptr<IChunkFactory> factory) {
    chunkFactory = std::move(factory);
}

void Terrain::initialize(std::shared_ptr<TerrainNoiseFactory> sharedNoiseFactory, std::function<void(float)> progressCallback) {
    if (!chunkFactory) {
        chunkFactory = std::make_shared<DefaultChunkFactory>();
    }    
    noiseFactory = sharedNoiseFactory;
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
            chunks[{x, z}] = chunkFactory->createChunk(x, z, shared_from_this());
            if (progressCallback) {
                float progress = static_cast<float>(++currentStep) / totalSteps;
                progressCallback(progress);
            }
        }
    }
}

const std::map<std::pair<int, int>, std::shared_ptr<Chunk>>& Terrain::getChunks() const {
    return chunks;
}

std::vector<std::shared_ptr<Chunk>> Terrain::getVisibleChunks(float playerX, float playerZ) const {
    std::vector<std::shared_ptr<Chunk>> visible;
    for (const auto& [coord, chunk] : chunks) {
        float dist = glm::distance(glm::vec2(coord.first * 16, coord.second * 16), glm::vec2(playerX, playerZ));
        if (dist <= TerrainConstants::TERRAIN_RENDER_DISTANCE) {
            visible.push_back(chunk);
        }
    }
    return visible;
}

void Terrain::loadChunk(int chunkX, int chunkZ)
{
    auto it = chunks.find({chunkX, chunkZ});
    if (it == chunks.end())
    {
        chunks[{chunkX, chunkZ}] = chunkFactory->createChunk(chunkX, chunkZ, shared_from_this());
    }
}

// void Terrain::render(Shader &shader, float playerX, float playerZ)
// {
//     const float renderDistance = TerrainConstants::TERRAIN_RENDER_DISTANCE;

//     for (auto &pair : chunks)
//     {
//         int chunkX = pair.first.first;
//         int chunkZ = pair.first.second;

//         float worldX = (chunkX + 0.5f) * ChunkConstants::SIZE; // Using 0.5f to set World position in middle of chunk.
//         float worldZ = (chunkZ + 0.5f) * ChunkConstants::SIZE;


//         float dx = worldX - playerX;
//         float dz = worldZ - playerZ;
//         float distanceSquared = dx * dx + dz * dz;

//         if (distanceSquared < renderDistance * renderDistance)
//         {
//             pair.second->render(shader);
//         }
//     }
// }



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

    const int viewDistance = TerrainConstants::VIEW_DISTANCE;  // Adjust based on desired chunk visibility

    // Load chunks explicitly
    for (int z = -viewDistance; z <= viewDistance; ++z)
    {
        for (int x = -viewDistance; x <= viewDistance; ++x)
        {
            int chunkX = playerChunkX + x;
            int chunkZ = playerChunkZ + z;

            if (chunks.find({chunkX, chunkZ}) == chunks.end())
                chunks[{chunkX, chunkZ}] = chunkFactory->createChunk(chunkX, chunkZ, shared_from_this());
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
            it = chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Terrain::updateChunksAroundPlayer(float playerX, float playerZ)
{
    int currentChunkX = static_cast<int>(floor(playerX / ChunkConstants::SIZE));
    int currentChunkZ = static_cast<int>(floor(playerZ / ChunkConstants::SIZE));

    if (lastPlayerChunk.first != currentChunkX || lastPlayerChunk.second != currentChunkZ)
    {
        updateChunks(playerX, playerZ);
        lastPlayerChunk = {currentChunkX, currentChunkZ};
    }
}

bool Terrain::hasChunksOnAllSides(int chunkX, int chunkZ) const
{
    auto currentChunk = chunks.find({chunkX, chunkZ});
    if (currentChunk == chunks.end())
    {
        // Current chunk doesn't exist; can't check neighbors reliably
        return false;
    }

    return chunks.find({chunkX + 1, chunkZ}) != chunks.end() &&
           chunks.find({chunkX - 1, chunkZ}) != chunks.end() &&
           chunks.find({chunkX, chunkZ + 1}) != chunks.end() &&
           chunks.find({chunkX, chunkZ - 1}) != chunks.end();
}