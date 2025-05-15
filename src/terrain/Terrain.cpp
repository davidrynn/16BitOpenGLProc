#include "Terrain.h"
#include <cmath>
#include <iostream>
#include "BiomeManager.h"
#include "ChunkConstants.h"
#include "DefaultChunkFactory.h"
#include "TerrainConstants.h"
#include "TerrainNoiseFactory.h"

static inline BiomeManager biomeManager;

TerrainType Terrain::getTerrainTypeAt(float worldX, float worldZ)
{
    Biome biome = biomeManager.getBiomeForPosition(worldX, worldZ);
    return biome.getDominantTerrain();
}

float Terrain::getHeightAt(float worldX, float worldZ)
{
    TerrainType terrainType = getTerrainTypeAt(worldX, worldZ);
    assert(noiseFactory && "TerrainNoiseFactory is null!");
    auto noiseFn = noiseFactory->getNoise(TerrainType::Mountains); // Use Plains noise for now
    float height = noiseFn ? noiseFn(worldX, worldZ) : 0.0f;
   // std::cout << "[Height] (" << worldX << ", " << worldZ << ") => " << height << std::endl;
    return height;
}

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