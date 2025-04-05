#include "BiomeManager.h"
#include <glm/gtc/random.hpp> // For linearRand

void BiomeManager::initialize(int numBiomes, int worldSize) {
    biomeCenters.clear();

    for (int i = 0; i < numBiomes; ++i) {
        float x = glm::linearRand(0.0f, static_cast<float>(worldSize));
        float z = glm::linearRand(0.0f, static_cast<float>(worldSize));
        
        TerrainType type = static_cast<TerrainType>(i % 2 == 0 ? TerrainType::Mountains : TerrainType::Plains);
        biomeCenters.emplace_back(glm::vec2(x, z), Biome(type));
    }
}

Biome BiomeManager::getBiomeForPosition(float x, float z) const {
    glm::vec2 pos(x, z);
    float minDistance = std::numeric_limits<float>::max();
    const Biome* closest = nullptr;

    for (const auto& [center, biome] : biomeCenters) {
        float distance = glm::distance(center, pos);
        if (distance < minDistance) {
            minDistance = distance;
            closest = &biome;
        }
    }

    return closest ? *closest : Biome(TerrainType::Plains); // Default fallback
}

// BiomeManager.cpp
TerrainType BiomeManager::getTerrainType(float x, float z) const {
    return getBiomeForPosition(x, z).getDominantTerrain();
}

const std::vector<std::pair<glm::vec2, Biome>>& BiomeManager::getBiomeCenters() const {
    return biomeCenters;
}

