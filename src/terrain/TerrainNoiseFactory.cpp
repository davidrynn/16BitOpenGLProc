#include "TerrainNoiseFactory.h"
#include <iostream>
#include "ConfigurableNoise.h"
#include "FastNoiseLiteWrapper.h"

TerrainNoiseFactory::TerrainNoiseFactory()
    : sharedBase(std::make_shared<FastNoiseLiteWrapper>())
{
    // Create noise generators for each terrain type with appropriate configurations
    auto plains = std::make_unique<ConfigurableNoise>(sharedBase, NoiseConfig::Plains());
    auto mountains = std::make_unique<ConfigurableNoise>(sharedBase, NoiseConfig::Mountains());
    auto desert = std::make_unique<ConfigurableNoise>(sharedBase, NoiseConfig::Desert());
    auto snow = std::make_unique<ConfigurableNoise>(sharedBase, NoiseConfig::Snow());

    // Store the noise functions
    heightFunctions[TerrainType::Plains] = [p = plains.get()](float x, float z) {
        return p->getNoise(x, z);
    };
    heightFunctions[TerrainType::Mountains] = [m = mountains.get()](float x, float z) {
        return m->getNoise(x, z);
    };
    heightFunctions[TerrainType::Desert] = [d = desert.get()](float x, float z) {
        return d->getNoise(x, z);
    };
    heightFunctions[TerrainType::Snow] = [s = snow.get()](float x, float z) {
        return s->getNoise(x, z);
    };

    // Store ownership of noise generators
    noiseInstances[TerrainType::Plains] = std::move(plains);
    noiseInstances[TerrainType::Mountains] = std::move(mountains);
    noiseInstances[TerrainType::Desert] = std::move(desert);
    noiseInstances[TerrainType::Snow] = std::move(snow);
}

std::function<float(float, float)> TerrainNoiseFactory::getNoise(TerrainType type) const {
    if (type == TerrainType::Count) {
        std::cerr << "Invalid TerrainType::Count passed to getNoise()" << std::endl;
        return [](float, float) { return 0.0f; };
    }

    auto it = heightFunctions.find(type);
    if (it != heightFunctions.end()) {
        return it->second;
    }

    std::cerr << "Error: No noise function registered for TerrainType "
              << static_cast<int>(type) << std::endl;
    return [](float, float) { return 0.0f; };
}
