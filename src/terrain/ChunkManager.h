#pragma once

#include <unordered_map>
#include <memory>
#include <list>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "IChunkFactory.h"
#include "TerrainThreadPool.h"

// Forward declare Terrain instead of including it
class Terrain;

class ChunkManager {
public:
    struct ChunkCoord {
        int x, z;
        
        bool operator==(const ChunkCoord& other) const {
            return x == other.x && z == other.z;
        }
    };
    
    struct ChunkCoordHash {
        std::size_t operator()(const ChunkCoord& coord) const {
            return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.z) << 1);
        }
    };

    explicit ChunkManager(TerrainThreadPool& threadPool, size_t maxChunks = 512);
    
    std::shared_ptr<Chunk> getChunk(int x, int z);
    void loadChunk(int x, int z, std::shared_ptr<Terrain> terrain);
    void unloadChunk(int x, int z);
    void updateLoadedChunks(const glm::vec3& playerPos, float viewDistance);
    
    // Set the terrain reference
    void setTerrain(std::shared_ptr<Terrain> terrain) { terrainRef = terrain; }
    
    // Debug/stats
    size_t getLoadedChunkCount() const { return loadedChunks.size(); }
    size_t getMaxChunks() const { return maxLoadedChunks; }

private:
    void unloadLeastRecentlyUsed();
    void updateLRU(const ChunkCoord& coord);

    std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>, ChunkCoordHash> loadedChunks;
    std::list<ChunkCoord> lruOrder;  // Front = least recently used, Back = most recently used
    size_t maxLoadedChunks;
    TerrainThreadPool& threadPool;
    std::weak_ptr<Terrain> terrainRef; // Store a weak_ptr to avoid circular reference
}; 