// TerrainNoiseFactory.cpp
#include "TerrainNoiseFactory.h"

#include <memory> // Required for std::make_unique
#include "MountainsNoise.h"
#include "PlainsNoise.h"
#include "TerrainType.h" // Ensure TerrainType is included
// ... other includes

std::unique_ptr<NoiseFunction> TerrainNoiseFactory::createNoiseFunction(TerrainType type) {
    switch (type) {
        case TerrainType::Mountains: return std::make_unique<MountainsNoise>();
        case TerrainType::Plains:    return std::make_unique<PlainsNoise>();
        // Add others
        default: return std::make_unique<PlainsNoise>();
    }
}
