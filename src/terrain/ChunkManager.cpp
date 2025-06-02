#include "ChunkManager.h"
#include "ChunkConstants.h"
#include "Debug.h"
#include "Terrain.h"
#include <algorithm>

ChunkManager::ChunkManager(TerrainThreadPool& threadPool, size_t maxChunks)
    : maxLoadedChunks(maxChunks)
    , threadPool(threadPool)
{
    Debug::log("ChunkManager initialized with " + std::to_string(maxChunks) + " max chunks");
}

std::shared_ptr<Chunk> ChunkManager::getChunk(int x, int z) {
    ChunkCoord coord{x, z};
    auto it = loadedChunks.find(coord);
    if (it != loadedChunks.end()) {
        updateLRU(coord);
        return it->second;
    }
    return nullptr;
}

void ChunkManager::loadChunk(int x, int z, std::shared_ptr<Terrain> terrain) {
    ChunkCoord coord{x, z};
    
    // Check if already loaded
    if (loadedChunks.find(coord) != loadedChunks.end()) {
        Debug::log("[ChunkManager] Chunk already loaded at (" + std::to_string(x) + ", " + std::to_string(z) + ")");
        updateLRU(coord);
        return;
    }
    
    // Enforce memory limits
    while (loadedChunks.size() >= maxLoadedChunks) {
        Debug::log("[ChunkManager] Max chunks reached, unloading least recently used chunk");
        unloadLeastRecentlyUsed();
    }
    
    try {
        if (!terrain || !terrain->chunkFactory) {
            Debug::logError("[ChunkManager] Invalid terrain or chunk factory");
            return;
        }
        
        // Create the chunk but don't generate it yet
        auto chunk = terrain->chunkFactory->createChunk(x, z, terrain);
        if (chunk) {
            loadedChunks[coord] = chunk;
            updateLRU(coord);
            
            // Queue the chunk for async generation
            Debug::log("[ChunkManager] Created chunk at (" + std::to_string(x) + ", " + std::to_string(z) + "), queueing for generation");
            threadPool.queueChunkUpdate(x, z, terrain);
        }
    } catch (const std::exception& e) {
        Debug::logError("[ChunkManager] Failed to load chunk: " + std::string(e.what()));
    }
}

void ChunkManager::unloadChunk(int x, int z) {
    ChunkCoord coord{x, z};
    auto it = loadedChunks.find(coord);
    if (it != loadedChunks.end()) {
        Debug::log("[ChunkManager] Unloading chunk at (" + std::to_string(x) + ", " + std::to_string(z) + ")");
        loadedChunks.erase(it);
        
        // Remove from LRU if present
        auto lruIt = std::find(lruOrder.begin(), lruOrder.end(), coord);
        if (lruIt != lruOrder.end()) {
            lruOrder.erase(lruIt);
        }
    }
}

void ChunkManager::updateLoadedChunks(const glm::vec3& playerPos, float viewDistance) {
    int playerChunkX = static_cast<int>(playerPos.x) / ChunkConstants::SIZE;
    int playerChunkZ = static_cast<int>(playerPos.z) / ChunkConstants::SIZE;
    float chunkViewDistance = viewDistance / ChunkConstants::SIZE;
    
    // Get terrain pointer
    auto terrain = terrainRef.lock();
    if (!terrain) {
        Debug::logError("[ChunkManager] No valid terrain reference");
        return;
    }
    
    // Collect chunks to unload
    std::vector<ChunkCoord> chunksToUnload;
    for (const auto& [coord, chunk] : loadedChunks) {
        float dx = static_cast<float>(coord.x - playerChunkX);
        float dz = static_cast<float>(coord.z - playerChunkZ);
        float distanceSquared = dx * dx + dz * dz;
        
        if (distanceSquared > chunkViewDistance * chunkViewDistance) {
            chunksToUnload.push_back(coord);
        }
    }
    
    // Rate limit: unload at most 4 chunks per update
    const size_t maxUnloadsPerUpdate = 4;
    size_t unloadCount = 0;
    
    // Sort by distance for priority unloading
    std::sort(chunksToUnload.begin(), chunksToUnload.end(),
        [playerChunkX, playerChunkZ](const ChunkCoord& a, const ChunkCoord& b) {
            float distA = std::pow(a.x - playerChunkX, 2) + std::pow(a.z - playerChunkZ, 2);
            float distB = std::pow(b.x - playerChunkX, 2) + std::pow(b.z - playerChunkZ, 2);
            return distA > distB; // Unload furthest chunks first
        });
    
    for (const auto& coord : chunksToUnload) {
        if (unloadCount >= maxUnloadsPerUpdate) break;
        unloadChunk(coord.x, coord.z);
        unloadCount++;
    }
    
    // Rate limit: load at most 4 chunks per update
    const size_t maxLoadsPerUpdate = 4;
    size_t loadCount = 0;
    
    // Load chunks in a spiral pattern from player outward
    int radius = static_cast<int>(chunkViewDistance);
    for (int r = 0; r <= radius && loadCount < maxLoadsPerUpdate; r++) {
        for (int x = -r; x <= r && loadCount < maxLoadsPerUpdate; x++) {
            for (int z = -r; z <= r && loadCount < maxLoadsPerUpdate; z++) {
                // Only process chunks at current radius
                if (std::abs(x) != r && std::abs(z) != r) continue;
                
                int chunkX = playerChunkX + x;
                int chunkZ = playerChunkZ + z;
                
                // Check if chunk should be loaded
                float dx = static_cast<float>(x);
                float dz = static_cast<float>(z);
                if (dx * dx + dz * dz <= chunkViewDistance * chunkViewDistance) {
                    auto it = loadedChunks.find({chunkX, chunkZ});
                    if (it == loadedChunks.end()) {
                        loadChunk(chunkX, chunkZ, terrain);
                        loadCount++;
                    }
                }
            }
        }
    }
}

void ChunkManager::unloadLeastRecentlyUsed() {
    if (!lruOrder.empty()) {
        const auto& lruChunk = lruOrder.front();
        Debug::log("[ChunkManager] Unloading LRU chunk at (" + std::to_string(lruChunk.x) + ", " + std::to_string(lruChunk.z) + ")");
        unloadChunk(lruChunk.x, lruChunk.z);
        lruOrder.pop_front();
    }
}

void ChunkManager::updateLRU(const ChunkCoord& coord) {
    // Remove if already in LRU
    auto it = std::find(lruOrder.begin(), lruOrder.end(), coord);
    if (it != lruOrder.end()) {
        lruOrder.erase(it);
    }
    
    // Add to back (most recently used)
    lruOrder.push_back(coord);
    
    // Trim if too large
    while (lruOrder.size() > maxLoadedChunks) {
        lruOrder.pop_front();
    }
} 