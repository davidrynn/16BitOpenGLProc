#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <map>
#include <GL/glew.h> 
#include "Chunk.h"
#include "Shader.h"

class Terrain {
public:
    static std::vector<float> generateTerrain(int gridSize);
    static std::vector<unsigned int> generateIndices(int gridSize);
    static float getHeightAt(float x, float z);
    void initialize();
    void render(Shader& shader);

private:
    std::map<std::pair<int, int>, Chunk*> chunks;
};

#endif