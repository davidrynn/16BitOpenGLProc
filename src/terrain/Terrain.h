#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <map>
#include <GL/glew.h> 
#include "BiomeManager.h"
#include "Chunk.h"
#include "Shader.h"
#include "TerrainType.h"

class Terrain {
public:
    static std::vector<float> generateTerrain(int gridSize, int chunkX, int chunkZ);
    static std::vector<unsigned int> generateIndices(int gridSize);
    static TerrainType getTerrainTypeAt(float worldX, float worldZ);
    static float getHeightAt(float worldX, float worldZ);
    static void generateChunkData(int chunkX, int chunkZ,
        std::vector<float>& outVertices,
        std::vector<unsigned int>& outIndices,
        std::vector<float>& outNormals);

    void initialize();
    void render(Shader& shader, float playerX, float playerZ);
    void Terrain::updateChunks(float playerX, float playerZ);

private:
    std::map<std::pair<int, int>, Chunk*> chunks;
};

#endif