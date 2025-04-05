#include "Biome.h"
#include <cmath>
Biome::Biome() {
    initializeWeights();
}

Biome::Biome(TerrainType dominant)
: dominantTerrain(dominant) {
    // Example: give 80% weight to the dominant terrain
    terrainWeights[dominant] = 0.8f;
    for (int i = 0; i < static_cast<int>(TerrainType::Count); ++i) {
        TerrainType type = static_cast<TerrainType>(i);
        if (type != dominant) {
            terrainWeights[type] = 0.2f / (static_cast<int>(TerrainType::Count) - 1);
        }
    }
}

TerrainType Biome::getTerrainType(float x, float z) const {
    float bestWeight = 0.0f;
    TerrainType best = TerrainType::Plains;
    for (const auto& [type, weight] : terrainWeights) {
        if (weight > bestWeight) {
            bestWeight = weight;
            best = type;
        }
    }
    return best;
}

void Biome::initializeWeights() {
    // Simple default: equal weights
    for (int i = 0; i < static_cast<int>(TerrainType::Count); ++i) {
        terrainWeights[static_cast<TerrainType>(i)] = 1.0f / static_cast<int>(TerrainType::Count);
    }
}

TerrainType Biome::getDominantTerrain() const {
    return dominantTerrain;
}
