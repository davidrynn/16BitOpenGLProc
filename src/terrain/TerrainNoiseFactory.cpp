#include "TerrainNoiseFactory.h"
#include <iostream> // for logging
#include "MountainsNoise.h"
#include "PlainsNoise.h"

std::unordered_map<TerrainType, std::unique_ptr<NoiseFunction>> TerrainNoiseFactory::noiseCache;

const NoiseFunction* TerrainNoiseFactory::getNoise(TerrainType type) {
    auto it = noiseCache.find(type);
    if (it != noiseCache.end()) {
        return it->second.get();
    }

    std::unique_ptr<NoiseFunction> noise;

    switch (type) {
        case TerrainType::Mountains:
            noise = std::make_unique<MountainsNoise>();
            break;
        case TerrainType::Plains:
            noise = std::make_unique<PlainsNoise>();
            break;
        default:
        std::cerr << "[WARN] getNoise: No noise function defined for TerrainType: "
        << static_cast<int>(type) << std::endl;
        return nullptr; // <-- Do NOT fallback silently
    }

    const NoiseFunction* ptr = noise.get();
    noiseCache[type] = std::move(noise);
    return ptr;
}
