#include "BiomeManager.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp> // For linearRand
#include "TerrainConstants.h"

void BiomeManager::initialize(int numBiomes, int worldSize) {
    biomeCenters.clear();

    for (int i = 0; i < numBiomes; ++i) {
        float x = glm::linearRand(0.0f, static_cast<float>(worldSize));
        float z = glm::linearRand(0.0f, static_cast<float>(worldSize));
        
        TerrainType type = static_cast<TerrainType>(i % 2 == 0 ? TerrainType::Mountains : TerrainType::Plains);
        biomeCenters.emplace_back(glm::vec2(x, z), Biome(type));
    }
}

const Biome& BiomeManager::getBiomeForPosition(float x, float z) const {
    float minDistSq = std::numeric_limits<float>::max();
    const Biome* closest = nullptr;

    glm::vec2 pos(x, z);
    for (const auto& [center, biome] : biomeCenters) {
        float distSq = glm::distance2(pos, center);
        if (distSq < minDistSq) {
            minDistSq = distSq;
            closest = &biome;
        }
    }

    return *closest;
}

TerrainType BiomeManager::getTerrainType(float x, float z) const {
    return getBiomeForPosition(x, z).getDominantTerrain();
}

const std::vector<std::pair<glm::vec2, Biome>>& BiomeManager::getBiomeCenters() const {
    return biomeCenters;
}

std::map<TerrainType, float> BiomeManager::getBiomeWeightsAt(float x, float z) const {
    std::map<TerrainType, float> weights;

    const float influenceRadius = 200.0f;
    float totalWeight = 0.0f;
    glm::vec2 pos(x, z);

    for (const auto& [center, biome] : biomeCenters) {
        float distSq = glm::distance2(center, pos);
        if (distSq < influenceRadius * influenceRadius) {
            float weight = 1.0f / (distSq + 1.0f); // +1 to avoid div by 0
            weights[biome.getDominantTerrain()] += weight;
            totalWeight += weight;
        }
    }

    // Normalize
    for (auto& [type, weight] : weights) {
        weight /= totalWeight;
    }

    return weights;
}