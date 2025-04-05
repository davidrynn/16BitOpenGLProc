#ifndef TERRAIN_NOISE_FACTORY_H
#define TERRAIN_NOISE_FACTORY_H

#include "TerrainType.h"
#include "NoiseFunction.h"
#include <memory>
#include <unordered_map>

class TerrainNoiseFactory {
public:
    static const NoiseFunction* getNoise(TerrainType type);

private:
    static std::unordered_map<TerrainType, std::unique_ptr<NoiseFunction>> noiseCache;
};

#endif
