#include "SnowNoise.h"

SnowNoise::SnowNoise(std::shared_ptr<BaseNoise> base)
    : baseNoise(std::move(base))
{
}

float SnowNoise::getNoise(float x, float z) const
{
    float base = baseNoise->getNoise(x * 0.03f, z * 0.03f); // lower freq
    return base * 2.0f; // flatter terrain
}