#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <map>
#include <functional>
#include <GL/glew.h>
#include "BiomeManager.h"
#include "Chunk.h"
#include "Shader.h"
#include "TerrainType.h"
#include "TerrainNoiseFactory.h"

class Terrain : public std::enable_shared_from_this<Terrain>
{
public:
    static std::vector<unsigned int> generateIndices(int gridSize);
   // std::shared_ptr<TerrainNoiseFactory> noiseFactory; 
   //TerrainNoiseFactory* noiseFactory = nullptr;
   std::shared_ptr<TerrainNoiseFactory> noiseFactory;
    static TerrainType getTerrainTypeAt(float worldX, float worldZ);
    float getHeightAt(float worldX, float worldZ);

    void initialize(std::shared_ptr<TerrainNoiseFactory> sharedNoiseFactory, std::function<void(float)> progressCallback);
    void updateChunksAroundPlayer(float playerX, float playerZ);
    void render(Shader &shader, float playerX, float playerZ);
    bool Terrain::hasChunksOnAllSides(int chunkX, int chunkZ) const;
private:
    std::map<std::pair<int, int>, Chunk *> chunks;
    std::pair<int, int> lastPlayerChunk = { INT_MIN, INT_MIN };
    void loadChunk(int chunkX, int chunkZ);
    void unloadFarChunks(int centerX, int centerZ, int radius);
    void updateChunks(float playerX, float playerZ);

};

#endif