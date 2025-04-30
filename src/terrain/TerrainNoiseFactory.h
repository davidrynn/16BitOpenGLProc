#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include "TerrainType.h"
#include "BaseNoise.h"

class TerrainNoiseFactory
{
public:
    TerrainNoiseFactory();

    // Prevent copying
    TerrainNoiseFactory(const TerrainNoiseFactory&) = delete;
    TerrainNoiseFactory& operator=(const TerrainNoiseFactory&) = delete;

    std::function<float(float, float)> getNoise(TerrainType type) const;

private:
    std::unordered_map<TerrainType, std::function<float(float, float)>> heightFunctions;
    std::unordered_map<TerrainType, std::unique_ptr<BaseNoise>> noiseInstances;
    std::shared_ptr<BaseNoise> sharedBase;
};
