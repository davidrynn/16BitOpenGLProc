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

class Terrain
{
public:
    static std::vector<unsigned int> generateIndices(int gridSize);
    static TerrainType getTerrainTypeAt(float worldX, float worldZ);
    static float getHeightAt(float worldX, float worldZ);

    void initialize(std::function<void(float)> progressCallback);
    void render(Shader &shader, float playerX, float playerZ);
    void Terrain::updateChunks(float playerX, float playerZ);

private:
    std::map<std::pair<int, int>, Chunk *> chunks;
};

#endif