#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>

class Terrain {
public:
    static std::vector<float> generateTerrain(int gridSize);
    static std::vector<unsigned int> generateIndices(int gridSize);
    static float getHeightAt(float x, float z);
};

#endif
