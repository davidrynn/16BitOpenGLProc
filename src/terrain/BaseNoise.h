#pragma once
#include <FastNoiseLite.h>

class BaseNoise {
protected:
    FastNoiseLite noise;

public:
    virtual float getNoise(float x, float z) const = 0;

    virtual void setFrequency(float freq) {
        noise.SetFrequency(freq);
    }

    FastNoiseLite& getNoiseGenerator() { return noise; } // expose if needed

    virtual float getRawNoise(float x, float z) const {
        return noise.GetNoise(x, z); // unscaled call
    }

    virtual ~BaseNoise() = default;
};

