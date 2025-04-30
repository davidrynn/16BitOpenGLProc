#pragma once
#include <FastNoiseLite.h>

class BaseNoise {
protected:
    FastNoiseLite noise;

public:
    virtual float getNoise(float x, float z) const = 0;
    virtual float getHeight(float x, float z) const {
        return noise.GetNoise(x, z) * 10.0f; // scale as needed
    }

    virtual ~BaseNoise() = default;
};
