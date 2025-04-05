#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Biome.h"

class BiomeManager {
public:
    void initialize(int numBiomes, int worldSize);
    Biome getBiomeForPosition(float x, float z) const;
    TerrainType getTerrainType(float x, float z) const;
    const std::vector<std::pair<glm::vec2, Biome>>& getBiomeCenters() const;

private:
    std::vector<std::pair<glm::vec2, Biome>> biomeCenters;
};
