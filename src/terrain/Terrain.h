#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <map>
#include <GL/glew.h> 
#include "Chunk.h"
#include "Shader.h"
#include "TerrainType.h"

class Terrain {
public:
    static std::vector<float> generateTerrain(int gridSize, int chunkX, int chunkZ);
    static std::vector<unsigned int> generateIndices(int gridSize);
    static TerrainType getTerrainTypeAt(int worldX, int worldZ);
    static float getHeightAt(float worldX, float worldZ);

    void initialize();
    void render(Shader& shader, float playerX, float playerZ);

private:
    std::map<std::pair<int, int>, Chunk*> chunks;
};

#endif