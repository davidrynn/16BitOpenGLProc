#pragma once

#include "BaseNoise.h"
#include <FastNoiseLite.h>  // Ensure FastNoiseLite is available in your include path

class FastNoiseLiteWrapper : public BaseNoise {
public:
    FastNoiseLiteWrapper();

    float getHeight(float x, float z) const override;
    float getNoise(float x, float z) const override; 
    void setSeed(int seed);
    void setFrequency(float frequency);
    void setType(FastNoiseLite::NoiseType type);

private:
    FastNoiseLite noise;
};
