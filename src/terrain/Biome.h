#pragma once

#include "TerrainType.h"
#include <map>

class Biome {
public:
    Biome(); // Default constructor
    Biome(TerrainType dominant); 

    TerrainType getTerrainType(float x, float z) const;
    TerrainType getDominantTerrain() const;

private:
    std::map<TerrainType, float> terrainWeights;
    void initializeWeights();
    TerrainType dominantTerrain;
};
