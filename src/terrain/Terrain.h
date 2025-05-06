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

class Terrain : public std::enable_shared_from_this<Terrain>
{
public:
    static std::vector<unsigned int> generateIndices(int gridSize);
    static TerrainType getTerrainTypeAt(float worldX, float worldZ);

    float getHeightAt(float worldX, float worldZ);
    void setChunkFactory(std::shared_ptr<IChunkFactory> factory);

    const std::map<std::pair<int, int>, std::shared_ptr<Chunk>>& getChunks() const;
    std::vector<std::shared_ptr<Chunk>> getVisibleChunks(float playerX, float playerZ) const;

    void initialize(std::shared_ptr<TerrainNoiseFactory> sharedNoiseFactory, std::function<void(float)> progressCallback);
    void updateChunksAroundPlayer(float playerX, float playerZ);
    // void render(Shader &shader, float playerX, float playerZ);
    bool Terrain::hasChunksOnAllSides(int chunkX, int chunkZ) const;
    std::shared_ptr<IChunkFactory> chunkFactory;

    
private:
    std::map<std::pair<int, int>, std::shared_ptr<Chunk>> chunks;
    std::shared_ptr<TerrainNoiseFactory> noiseFactory;

    std::pair<int, int> lastPlayerChunk = { INT_MIN, INT_MIN };
    
    void loadChunk(int chunkX, int chunkZ);
    void unloadFarChunks(int centerX, int centerZ, int radius);
    void updateChunks(float playerX, float playerZ);

};

#endif