#pragma once
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "Biome.h"
#include "TerrainType.h"

class BiomeManager {
public:
    void initialize(int numBiomes, int worldSize);
    const Biome& getBiomeForPosition(float x, float z) const;
    TerrainType getTerrainType(float x, float z) const;
    const std::vector<std::pair<glm::vec2, Biome>>& getBiomeCenters() const;
    std::map<TerrainType, float> getBiomeWeightsAt(float x, float z) const;


private:
    std::vector<std::pair<glm::vec2, Biome>> biomeCenters;
};
