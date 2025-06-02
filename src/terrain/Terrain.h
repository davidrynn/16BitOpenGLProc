#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <map>
#include <functional>
#include "BiomeManager.h"
#include "Chunk.h"
#include "IChunkFactory.h"
#include "TerrainType.h"
#include "TerrainNoiseFactory.h"

// Forward declarations
class ChunkManager;
struct TerrainImpl;
class TerrainThreadPool;

class Terrain : public std::enable_shared_from_this<Terrain>
{
public:
    explicit Terrain(TerrainThreadPool& threadPool);
    ~Terrain();
    
    static TerrainType getTerrainTypeAt(float worldX, float worldZ);

    float getHeightAt(float worldX, float worldZ);
    void setChunkFactory(std::shared_ptr<IChunkFactory> factory);

    const std::map<std::pair<int, int>, std::shared_ptr<Chunk>>& getChunks() const;
    std::vector<std::shared_ptr<Chunk>> getVisibleChunks(float playerX, float playerZ) const;

    void initialize(std::shared_ptr<TerrainNoiseFactory> sharedNoiseFactory, std::function<void(float)> progressCallback);
    void updateChunksAroundPlayer(float playerX, float playerZ);
    bool hasChunksOnAllSides(int chunkX, int chunkZ) const;
    std::shared_ptr<IChunkFactory> chunkFactory;

private:
    void initializeChunkManager();
    void loadChunk(int chunkX, int chunkZ);
    void unloadFarChunks(int centerX, int centerZ, int radius);
    void updateChunks(float playerX, float playerZ);

    std::map<std::pair<int, int>, std::shared_ptr<Chunk>> chunks;
    std::shared_ptr<TerrainNoiseFactory> noiseFactory;
    std::unique_ptr<TerrainImpl> impl;
    std::pair<int, int> lastPlayerChunk = { INT_MIN, INT_MIN };
};

#endif