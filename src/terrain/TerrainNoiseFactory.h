#ifndef TERRAIN_NOISE_FACTORY_H
#define TERRAIN_NOISE_FACTORY_H

#include "NoiseFunction.h"
#include "TerrainType.h"
#include <memory>

class TerrainNoiseFactory {
public:
    static std::unique_ptr<NoiseFunction> createNoiseFunction(TerrainType type);
};

#endif