#include "Terrain.h"
#include <cmath>
#include <iostream>
#include "BiomeManager.h"
#include "ChunkConstants.h"
#include "DefaultChunkFactory.h"
#include "TerrainConstants.h"
#include "TerrainNoiseFactory.h"
#include "ChunkManager.h"
#include "TerrainThreadPool.h"

static inline BiomeManager biomeManager;

struct TerrainImpl {
    std::unique_ptr<ChunkManager> chunkManager;
    TerrainImpl(TerrainThreadPool& threadPool) 
        : chunkManager(std::make_unique<ChunkManager>(threadPool, 512)) {} // Limit to 512 chunks max
};

Terrain::Terrain(TerrainThreadPool& threadPool) 
    : impl(std::make_unique<TerrainImpl>(threadPool)) 
{
    // Constructor should only do basic initialization
}

void Terrain::initializeChunkManager()
{
    // Now it's safe to call shared_from_this() since we're outside the constructor
    impl->chunkManager->setTerrain(shared_from_this());
}

Terrain::~Terrain() = default;

TerrainType Terrain::getTerrainTypeAt(float worldX, float worldZ)
{
    Biome biome = biomeManager.getBiomeForPosition(worldX, worldZ);
    return biome.getDominantTerrain();
}

float Terrain::getHeightAt(float worldX, float worldZ)
{
    TerrainType terrainType = getTerrainTypeAt(worldX, worldZ);
    assert(noiseFactory && "TerrainNoiseFactory is null!");
    
    // Get the noise function for the actual terrain type
    auto noiseFn = noiseFactory->getNoise(terrainType);
    
    // Get biome weights for potential blending
    auto biomeWeights = biomeManager.getBiomeWeightsAt(worldX, worldZ);
    
    // If we have multiple biomes influencing this point, blend their heights
    if (biomeWeights.size() > 1) {
        float totalHeight = 0.0f;
        float totalWeight = 0.0f;
        
        for (const auto& [type, weight] : biomeWeights) {
            auto typeNoiseFn = noiseFactory->getNoise(type);
            if (typeNoiseFn) {
                totalHeight += typeNoiseFn(worldX, worldZ) * weight;
                totalWeight += weight;
            }
        }
        
        // Normalize if we have valid weights
        if (totalWeight > 0.0f) {
            return totalHeight / totalWeight;
        }
    }
    
    // Fall back to single terrain type if no blending needed
    return noiseFn ? noiseFn(worldX, worldZ) : 0.0f;
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

    // Initialize the ChunkManager with our shared_ptr
    initializeChunkManager();

    // Load immediate chunks around spawn first
    const int initialRadius = 2;
    const int totalSteps = (2 * initialRadius + 1) * (2 * initialRadius + 1);
    int currentStep = 0;

    // First pass: Load the immediate chunks (3x3 grid)
    for (int z = -1; z <= 1; ++z) {
        for (int x = -1; x <= 1; ++x) {
            auto chunk = chunkFactory->createChunk(x, z, shared_from_this());
            chunks[{x, z}] = chunk;
            // Generate and upload immediately for spawn chunks
            chunk->generate();
            chunk->uploadToGPU();
            if (progressCallback) {
                float progress = static_cast<float>(++currentStep) / totalSteps * 0.5f;
                progressCallback(progress);
            }
        }
    }

    // Second pass: Load the remaining chunks in the view distance
    for (int z = -initialRadius; z <= initialRadius; ++z) {
        for (int x = -initialRadius; x <= initialRadius; ++x) {
            if (std::abs(x) <= 1 && std::abs(z) <= 1) continue; // Skip already loaded chunks
            chunks[{x, z}] = chunkFactory->createChunk(x, z, shared_from_this());
            if (progressCallback) {
                float progress = 0.5f + static_cast<float>(++currentStep) / totalSteps * 0.5f;
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

void Terrain::updateChunks(float playerX, float playerZ)
{
    int playerChunkX = static_cast<int>(floor(playerX / ChunkConstants::SIZE));
    int playerChunkZ = static_cast<int>(floor(playerZ / ChunkConstants::SIZE));

    const int viewDistance = TerrainConstants::VIEW_DISTANCE;

    // Only use ChunkManager to handle loading/unloading
    impl->chunkManager->updateLoadedChunks(
        glm::vec3(playerX, 0, playerZ),
        viewDistance * ChunkConstants::SIZE
    );

    // Remove chunks that are no longer in the ChunkManager
    for (auto it = chunks.begin(); it != chunks.end();) {
        int chunkX = it->first.first;
        int chunkZ = it->first.second;

        if (!impl->chunkManager->getChunk(chunkX, chunkZ)) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }

    // Update our chunks map with any new chunks from the manager
    for (int z = -viewDistance; z <= viewDistance; ++z) {
        for (int x = -viewDistance; x <= viewDistance; ++x) {
            int chunkX = playerChunkX + x;
            int chunkZ = playerChunkZ + z;

            auto chunk = impl->chunkManager->getChunk(chunkX, chunkZ);
            if (chunk && chunks.find({chunkX, chunkZ}) == chunks.end()) {
                chunks[{chunkX, chunkZ}] = chunk;
            }
        }
    }
}

void Terrain::loadChunk(int chunkX, int chunkZ)
{
    // First check if chunk exists in manager
    auto chunk = impl->chunkManager->getChunk(chunkX, chunkZ);
    if (!chunk) {
        // Load through manager if not exists
        impl->chunkManager->loadChunk(chunkX, chunkZ, shared_from_this());
        chunk = impl->chunkManager->getChunk(chunkX, chunkZ);
        if (chunk) {
            chunks[{chunkX, chunkZ}] = chunk;
        }
    }
}

void Terrain::unloadFarChunks(int centerX, int centerZ, int radius)
{
    std::vector<std::pair<int, int>> chunksToUnload;
    
    for (const auto& [coord, chunk] : chunks) {
        int dx = coord.first - centerX;
        int dz = coord.second - centerZ;
        float distance = std::sqrt(dx * dx + dz * dz);
        
        if (distance > radius) {
            chunksToUnload.push_back(coord);
        }
    }
    
    for (const auto& coord : chunksToUnload) {
        impl->chunkManager->unloadChunk(coord.first, coord.second);
        chunks.erase(coord);
    }
}

void Terrain::updateChunksAroundPlayer(float playerX, float playerZ)
{
    int currentChunkX = static_cast<int>(floor(playerX / ChunkConstants::SIZE));
    int currentChunkZ = static_cast<int>(floor(playerZ / ChunkConstants::SIZE));

    // Always update chunks if we haven't loaded all neighbors
    bool needsUpdate = !hasChunksOnAllSides(currentChunkX, currentChunkZ);
    
    // Or if we've moved to a new chunk
    needsUpdate |= (lastPlayerChunk.first != currentChunkX || lastPlayerChunk.second != currentChunkZ);

    if (needsUpdate)
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